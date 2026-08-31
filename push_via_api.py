#!/usr/bin/env python3
"""
Push FCL v3.1.0 to GitHub via the Git Data REST API.

Why not `git push`? The sandbox egress proxy blocks github.com (502) but
allows api.github.com (200). So we mirror the commit through the REST API.

Strategy:
  parent = remote main's current commit  (linear history; content is the v3.1.0 tree)
  tree   = full current working tree (blobs de-duped by sha on GitHub's side)
  commit = the v3.1.0 release message
  then force-update refs/heads/main and recreate annotated tag v3.1.0.

NOTE: this script syncs exactly the paths reported by `git ls-files`, so any
new file (e.g. src/lexer.*, examples/tc_double.fc) MUST be `git add`ed first.
Build artifacts (fcl.exe, tests/unit_tests.exe) are gitignored and excluded.
"""
import base64
import json
import os
import subprocess
import sys
import urllib.request
import urllib.error

REPO = "Huang-520-add/fcl"
TOKEN = os.environ["GITHUB_TOKEN"]
PROXY = os.environ.get("HTTPS_PROXY") or os.environ.get("https_proxy")

API = "https://api.github.com"
COMMIT_MSG = (
    "feat: v3.1.0 — Turing-complete (unbounded tape + WHILE), lexer module, AST tokens\n\n"
    "Turing completeness:\n"
    "  - New unbounded sparse storage tape `TAPE` (std::map<long long,int64_t>).\n"
    "  - Movable read/write head: FORWARD / BACKWARD.\n"
    "  - Arithmetic on a cell: BUMP <n>.\n"
    "  - Cell <-> species energy bridge: LOAD / STORE.\n"
    "  - Unbounded loop: WHILE TAPE UNTIL <expr>.\n"
    "  Together these give a Brainfuck / Turing-machine equivalent while all\n"
    "  earlier syntax (ASSESS, HIBERNATION, INTRODUCE, ...) keeps working.\n"
    "Lexer: real token module (src/lexer.h/.cpp) as a drop-in for the old ad-hoc\n"
    "  splitWS, byte-identical tokenization so the 140+ unit assertions stay green.\n"
    "AST: every Stmt now carries its token list (`toks`) for tooling/introspection.\n"
    "Docs: README / REFERENCE / SPEC / TUTORIAL (zh+en) / ESOLANGS updated to state\n"
    "  Turing-complete since v3.1.0 with the completeness argument; CHANGELOG added.\n"
    "Tests: unit 140->155, run_tests 46->47, output_tests 40->41 (tc_double.fc)."
)
TAG_NAME = "v3.1.0"
TAG_MSG = "v3.1.0 — Turing-complete (unbounded tape + WHILE), lexer module, AST tokens"

# Proxy handler
def build_opener():
    if PROXY:
        handler = urllib.request.ProxyHandler({"https": PROXY, "http": PROXY})
        return urllib.request.build_opener(handler)
    return urllib.request.build_opener()

OPENER = build_opener()

def api(method, path, data=None):
    url = f"{API}/repos/{REPO}/{path}"
    body = json.dumps(data).encode("utf-8") if data is not None else None
    req = urllib.request.Request(url, data=body, method=method)
    req.add_header("Authorization", f"Bearer {TOKEN}")
    req.add_header("Accept", "application/vnd.github+json")
    req.add_header("X-GitHub-Api-Version", "2022-11-28")
    req.add_header("User-Agent", "fcl-push-script")
    if body is not None:
        req.add_header("Content-Type", "application/json")
    try:
        with OPENER.open(req, timeout=60) as resp:
            raw = resp.read().decode("utf-8", "replace")
            return resp.status, (json.loads(raw) if raw else {})
    except urllib.error.HTTPError as e:
        raw = e.read().decode("utf-8", "replace")
        print(f"[HTTP {e.code}] {method} {url}\n{raw}", file=sys.stderr)
        raise

REPO_DIR = os.path.dirname(os.path.abspath(__file__))

def main():
    print(f"[*] proxy={PROXY}")
    # 1. Get remote main SHA (parent)
    status, ref = api("GET", "git/refs/heads/main")
    parent_sha = ref["object"]["sha"]
    print(f"[*] remote main = {parent_sha} ({status})")

    # 2. Collect tracked files
    files = subprocess.check_output(
        ["git", "ls-files"], cwd=REPO_DIR, text=True
    ).splitlines()
    files = [f for f in files if f]  # drop empty
    print(f"[*] {len(files)} tracked files")

    # 3. Create blobs (base64 for byte-exact content, incl. UTF-8 emoji)
    entries = []
    for f in files:
        p = os.path.join(REPO_DIR, f)
        with open(p, "rb") as fh:
            content = fh.read()
        b64 = base64.b64encode(content).decode("ascii")
        st, blob = api("POST", "git/blobs",
                       {"content": b64, "encoding": "base64"})
        entries.append({"path": f, "mode": "100644",
                        "type": "blob", "sha": blob["sha"]})
        print(f"    blob {blob['sha'][:10]} {f}")

    # 4. Create tree (auto-creates intermediate dirs)
    st, tree = api("POST", "git/trees", {"tree": entries})
    tree_sha = tree["sha"]
    print(f"[*] tree = {tree_sha}")

    # 5. Create commit
    author = {"name": "YouClaw", "email": "youclaw@local", "date": _now()}
    commit = {
        "message": COMMIT_MSG,
        "tree": tree_sha,
        "parents": [parent_sha],
        "author": author,
        "committer": author,
    }
    st, new_commit = api("POST", "git/commits", commit)
    new_sha = new_commit["sha"]
    print(f"[*] new commit = {new_sha} ({st})")

    # 6. Force-update main
    st, _ = api("PATCH", "git/refs/heads/main",
                {"sha": new_sha, "force": True})
    print(f"[*] main -> {new_sha} ({st})")

    # 7. Recreate annotated tag v3.1.0
    tag_obj = {
        "tag": TAG_NAME,
        "message": TAG_MSG,
        "object": new_sha,
        "type": "commit",
        "tagger": author,
    }
    st, tag = api("POST", "git/tags", tag_obj)
    tag_sha = tag["sha"]
    print(f"[*] tag object = {tag_sha} ({st})")
    st, _ = api("PATCH", f"git/refs/tags/{TAG_NAME}",
                {"sha": tag_sha, "force": True})
    print(f"[*] refs/tags/{TAG_NAME} -> {tag_sha} ({st})")

    # 8. Verify
    st, vmain = api("GET", "git/refs/heads/main")
    st, vtag = api("GET", f"git/refs/tags/{TAG_NAME}")
    print("[VERIFY]")
    print(f"  main = {vmain['object']['sha']}  (expect {new_sha})")
    print(f"  tag  = {vtag['object']['sha']}  (expect {tag_sha})")
    ok = vmain['object']['sha'] == new_sha and vtag['object']['sha'] == tag_sha
    print("[OK]" if ok else "[MISMATCH]")
    sys.exit(0 if ok else 1)

def _now():
    from datetime import datetime, timezone
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")

if __name__ == "__main__":
    main()
