# HLD card -> PNG kahani (LOCKED format, sample = 02_rate_limiter) — guide for one design file

All text Hinglish (Hindi in Roman script), simple words, NO emoji. Repo root: C:\DSA_PRACTICE. Shell: Git Bash.

## 0. Read first (mandatory)
- `04_HLD/SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md` (cards 1-9 = finished target format)
- `04_HLD/SYSTEM_DESIGNS/02_rate_limiter/img/c2_anim.js`, `c4_anim.js`, `c6_anim.js`, `c9_anim.js` (finished scenes)
- `04_HLD/tools/anim_base.js` (helpers) — and LOOK at `02_rate_limiter/img/c4_1.png ... c4_7.png`, `c2_4.png` to see the style.
- Then read YOUR design md completely, and its existing `img/cN_anim.js` (old GIF scenes, if any) to reuse layout ideas.

## 1. What to build per card N (every numbered card `## N. ...` in your md)
Scenes in this order: DIKKAT -> ILAAJ -> AGAR x n -> WAPAS.
- DIKKAT = the card heading's bug happening (bg RED, caption `DIKKAT: ...`).
- ILAAJ = the HAL line's fix working (bg GRN, caption `ILAAJ: ...` = HAL words, same order, fewer words if needed).
- AGAR = ONLY the card's agar / cross-Q items that can be SHOWN: something gira / dheema / band / wapas aaya /
  do ek saath / galat hua / door. Each such item = one scene (bg YEL, caption `AGAR <sawaal>? -> <jawab>`).
  Pure-fact items (header name, timeout 5 ms, command name, "kahan lagega", numbers only) = NO scene (text only).
- WAPAS = thing came back / back to normal (bg GRN, caption `WAPAS: ...`). Take it from the card's own text (an agar
  like "X wapas aaya", or HAL/KYUN). Never invent a new fact. If truly nothing fits, a WAPAS that shows the ILAAJ
  state steady after the last AGAR is ok (words from HAL).
- Typical card = 3-6 scenes. Max 7.
- Captions: ONE line, <= 75 characters INCLUDING the prefix. Same names / numbers as the md. No new numbers or claims.

## 2. Scene file `img/cN_anim.js` (overwrite old GIF scene if exists; create if not)
```js
// Card N: <one line what happens>
const DUR = <3 x scenes>;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ...'],
  [3, GRN, 'ILAAJ', 'ILAAJ: ...'],
  [6, YEL, 'AGAR', 'AGAR ...? -> ...'],
  [9, GRN, 'WAPAS', 'WAPAS: ...'],
];
function setup() {}
function frame(ph) { if (ph === 0) {...} else if (ph === 1) {...} ... }   // draws the FINAL state of that scene
```
Canvas: 900 x 400, caption bar on top; draw only inside x 10..890, y 10..345 (scene coords).
Helpers (anim_base.js): `B(x,y,w,h,label,sub,{st:'bad'|'good'|'acc'|'dim'})` box · `A(x1,y1,x2,y2,color,{n,at,dash})`
arrow (+ kram badge n at fraction at) · `AP([[x,y],...],color,{n,at,dash})` bent arrow · `num(x,y,n,color)` badge ·
`L(x,y,text,color,anchor)` small "kya hua" label · `dots(n,x,y,color|[colors],{gap,cols,hollow})` · `dot([x,y],c,'',label,lc)`
· `cross(x,y)` · `bar(x,y,w,h,frac,label,forceGreen)` · `cells(x,y,[[text,color],..],{w,h})` · `T(x,y,text,{c,b,size,a,halo})`
· colors `C.msg C.good C.bad C.acc C.muted C.line`, backgrounds `RED GRN YEL`.

## 3. Picture rules (Arpan + Fable, HARD)
1. TASVEER ZYADA, TEXT KAM: boxes / arrows / X / dots / bars tell the story. Box label = only name (+ short sub like
   `ginti: 5`, `CPU`, `LOCK`). No paragraphs. Should be understandable without reading the caption.
2. KRAM: every arrow that DOES something in that scene gets a badge `{n: k}` in event order 1, 2, 3 ... (same colour
   as the arrow). Parallel same step = same number. Arrows that do nothing = no badge (draw in C.line).
   Inside-box events (crash, loop, switch) get a `num()` badge too, in kram.
3. Every line has an arrowhead (use A / AP, never a bare line for a flow).
4. AGAR scene shows the CAUSE, not only the result: put an `L()` label next to the thing that changed / broke
   ("gira", "switch ho raha", "CPU full, gira nahi", "copy bhi gayi", "ghar door"...). Build AGAR scenes on the SAME
   boxes / layout as the ILAAJ scene of that card, so the viewer sees what changed.
5. Colours: bug = red, fix = green, request = blue, in-progress / warning = orange (acc).
6. Never overlap: a label / badge / dot must not sit on box text, on another label, or on another badge. Crosses go
   outside box edges. Check every PNG.
7. Use the same people / numbers as the md (Ravi, Arpan, 30k, 429 ...).
8. (Arpan) NO LONELY NUMBER: next to every badge the viewer must see WHAT that step did and WHY (a short L() label
   like 'apni ginti: 5 <= 5, andar', 'site band', or a visible result). A badge on a stub arrow with nothing beside it
   = confusing. The CAUSE (gira / crash / loop) gets a number too, first in kram.
9. (Arpan) AGAR scene must also SHOW the jawab / hal, not only the problem: green L() label 'hal: ...' with words from
   the md answer (even 'hal: chalta hai, ...'). Skip only if the very next WAPAS scene visibly shows that exact fix.
11. (Arpan) TEER KI DISHA = cheez jidhar jaati: paisa / request / khabar jis se nikli wahan se, jahan pahunchi wahan tak.
    Bhejne wala baayein, paane wala daayein. Ulta teer (jaise paisa Payment Svc se Arpan ko) = galat.
12. (Arpan) Sirf wahi dabba jo us card / us scene ki baat me hai. Jo cheez file me aage ke card me aati hai (jaise
    ledger card 7 me) wo pehle ke card me nahi. AGAR me koi dabba aaye to sirf usi scene me.
13. Har scene ko caption ke saath padho: kya tasveer wahi kahani bol rahi hai? Sirf overlap check kaafi nahi.


## 4. Build + self-check (do it for EVERY card, fix until clean)
- From `C:\DSA_PRACTICE\04_HLD`: `python tools/anim_build.py SYSTEM_DESIGNS/<d>/img/c1_anim.js SYSTEM_DESIGNS/<d>/img/c2_anim.js`
  (max 3 scene files per call; it writes `img/cN_1.png, cN_2.png ...` next to the js and `cN_anim.html`).
- Contact sheet of one card (2 columns) — run inside the img folder:
  `bash <your scratch dir>/../sheet.sh N` is NOT available to you; instead use:
  ```bash
  n=4; fs=$(ls c${n}_[0-9].png | sort -V); k=$(echo "$fs"|wc -l); a=""; for f in $fs; do a="$a -i $f"; done
  if [ $((k%2)) -eq 1 ]; then a="$a -f lavfi -i color=c=black:s=900x400:d=1"; k=$((k+1)); fi
  fc=""; r=0; for ((i=0;i<k;i+=2)); do fc="$fc[$i:v][$((i+1)):v]hstack[r$r];"; r=$((r+1)); done
  rows=""; for ((i=0;i<r;i++)); do rows="$rows[r$i]"; done
  ffmpeg -y -loglevel error $a -filter_complex "${fc}${rows}vstack=inputs=$r" -frames:v 1 <SCRATCH>/sheet_c$n.png
  ```
  (if only 2 scenes: `ffmpeg -y -loglevel error -i c${n}_1.png -i c${n}_2.png -filter_complex hstack <SCRATCH>/sheet_c$n.png`)
- LOOK at every sheet with the Read tool. Check rules 1-7. Fix, rebuild, look again. Do not report done unlooked.
- Caption length check: every caption <= 75 chars (write a tiny python check over your cN_anim.js files).

## 5. md changes (only inside numbered cards; all else byte-for-byte same; keep CRLF/LF as the file has)
Per card, right after the `Cue: ...` line (replace an existing `![...](img/cN_anim.gif)` line if there is one):
```
Cue: ...

<details><summary>kahani dekho (K scene)</summary>

<img src="img/cN_1.png" width="900">

<img src="img/cN_2.png" width="900">

</details>

```
(K = scene count; blank line between images is required; use `<img>` NOT `![]()` — `![]()` does not render inside details.)
Inside the card's monospace block:
- Remove the `AGAR` section. Each agar goes UNDER the HAL line it comes from, in scene order, as
  `        ↳ agar <sawaal>? -> <jawab>` (8 spaces then ↳). Long: question on that line, answer on next line
  `          -> <jawab>` (10 spaces). For an agar that has a scene, the ↳ line uses the SAME words as the scene caption;
  any extra fact the old answer had goes on a continuation line (10 spaces) so nothing is lost.
- Fact-only agar (no scene) also becomes a ↳ line, same facts.
- An agar that does not come from any HAL line -> moved as a plain sentence into the file's `## Pooche to` section
  (create none; if that section doesn't exist, keep it as ↳ under HAL).
- HAL / KYUN / NAHI and any other label lines stay as they are. If HAL has 2 lines, ↳ lines go under the line they belong to.
- Every line inside the block <= 110 characters. Full spoken Hinglish sentences, no shorthand.
- `TOOTA` lines (if a card has one): keep the text as is, AND the TOOTA story becomes the DIKKAT scene of that card.
- Top header note: replace the line starting `> Mock me sirf heading + Cue + GIF` with exactly
  `> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card ek block: HAL (↳ agar = cross-Q) · KYUN · NAHI (doosra kyun nahi).`
- Shuru / other header lines / Poori tasveer / Poora design / 10 guna / Pooche to (except moved agar) / HANDS-ON: unchanged.

## 5b. Files that never had GIFs
Build scenes only from what the card's DIKKAT (heading / TOOTA) / HAL / agar lines say. Nothing invented.

## 6. Cleanup
- Delete this design's old `img/cN_anim.gif` files (git rm not needed, just delete the files). Keep .svg/.mmd/.json.
- Do NOT git add / commit / push. The lead reviews and pushes.

## 7. Report (short, plain)
Per card: scene count + one line per scene (kind + caption). Which sheets you looked at, what you fixed.
md: count of ↳ lines, anything moved to Pooche to, any place you deviated from this guide and why.

14. AGAR scene ka caption aur md ki "↳ agar" line SHABD-BA-SHABD same (Fable, 20-Oct). Pehle caption likho, wahi md me copy.
15. Ek kadam = ek number. Do cheez ek saath (A + B me likhna) to number sirf ek teer pe.
16. "Pehle ye phir wo" (race / kram) wali kahani = 4 frame baayein se daayein, har frame ek pal + us pal ki haalat.
