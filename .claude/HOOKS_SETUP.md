# .claude HOOKS SETUP (reproducible reference)

> `.claude/settings.local.json` git-tracked NAHI hai (untracked -- harness usme har command pe permission auto-add karta,
> baar-baar "modified" dikhta tha = noise). Us file ka HOOKS portion yahaan documented hai taaki kabhi reset ho to
> dobara set kar sakein. (Content files -- DSA_GUARD.md, MEMORY-ANCHORS.md -- alag se safe hain.)

## Do hooks (settings.local.json ke "hooks" block me):

1. **SessionStart** -> har naye session pe `MEMORY-ANCHORS.md` auto-load (positive north-star + good-baatein):
   ```json
   "SessionStart": [
     { "matcher": "",
       "hooks": [ { "type": "command",
         "command": "cat '/c/Users/ArpanMaheshwari/.claude/projects/C--DSA-PRACTICE/memory/MEMORY-ANCHORS.md'",
         "shell": "bash", "timeout": 10 } ] }
   ]
   ```

2. **UserPromptSubmit** -> har message pe `DSA_GUARD.md` inject (DSA behavioral rules):
   ```json
   "UserPromptSubmit": [
     { "hooks": [ { "type": "command",
         "command": "cat /c/DSA_PRACTICE/.claude/DSA_GUARD.md",
         "shell": "bash" } ] }
   ]
   ```

## Verify:
- SessionStart theek -> MEMORY-ANCHORS.md ka `HOOK_SENTINEL ... MOTU_BHAI` comment session-start pe context me dikhta hai (bina file Read kiye).
- UserPromptSubmit theek -> har message ke saath DSA_GUARD ki lines inject hoti hain.
