"""Scene (cN_anim.js) + anim_base.js -> cN_anim.html -> GIF (ya PH wala scene -> cN_1.png, cN_2.png ...).
python 04_HLD/tools/anim_build.py <scene.js> [<scene.js> ...]"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
TPL = '''<!doctype html>
<!-- banaya: tools/anim_build.py se. Badlaav scene .js me karo, ye file dobara banti hai. -->
<meta charset="utf-8">
<style>body{{margin:0;background:#11161c}} svg text{{font-family:Consolas,"Courier New",monospace}}</style>
<svg width="900" height="400" viewBox="0 0 900 400" xmlns="http://www.w3.org/2000/svg">
<rect width="900" height="400" fill="#11161c"/>
<rect id="capbg" x="0" y="0" width="900" height="44" fill="#3a1414"/>
<text id="cap" x="20" y="29" font-size="15" font-weight="bold" fill="#e6edf3"></text>
<g transform="translate(0,40)"><g id="st"></g><g id="dyn"></g></g>
</svg>
<script>
{base}
{scene}
setup(); draw(0);
</script>
'''

base = open(os.path.join(HERE, 'anim_base.js'), encoding='utf-8').read()
for js in sys.argv[1:]:
    scene = open(js, encoding='utf-8').read()
    html = js[:-3] + '.html'
    open(html, 'w', encoding='utf-8').write(TPL.format(base=base, scene=scene))
    if 'const PH' in scene:   # kahani wala scene: har phase ka aakhri frame PNG, GIF nahi
        subprocess.run(['node', os.path.join(HERE, 'anim_frames.js'), os.path.dirname(os.path.abspath(js)), html],
                       check=True)
    else:
        subprocess.run(['node', os.path.join(HERE, 'anim2gif.js'), html, '12'], check=True)
