"""Card me poster lagao: python 04_HLD/tools/poster_md.py <design.md> <cardNo> [v]
Poster (img/cN_poster.png) Jod line ke neeche (Jod nahi to Rail / Cue line ke neeche).
Purani kahani (<details> kahani dekho) aur Cross-Q list ([!TIP]) hatati hai. Pehle list ka har sawaal poster me daalo."""
import sys
p, n = sys.argv[1], int(sys.argv[2])
v = sys.argv[3] if len(sys.argv) > 3 else '1'
s = open(p, encoding='utf-8').read()
i = s.index(f'\n## {n}. ') + 1
j = s.find(f'\n## {n + 1}. ', i)
j = j + 1 if j != -1 else s.index('\n## Poora design', i) + 1
c = s[i:j]
assert 'poster.png' not in c, 'poster pehle se hai'
k = c.find('\nJod: ')
if k == -1:
    k = c.index('\n**Rail')
ke = c.index('\n', k + 1) + 1
c = c[:ke] + f'\n<img src="img/c{n}_poster.png?v={v}" width="1000" alt="card {n}: kya toota, kaise bacha">\n' + c[ke:]
if '<details><summary>kahani dekho' in c:
    d0 = c.index('<details><summary>kahani dekho')
    d1 = c.index('</details>\n', d0) + len('</details>\n')
    c = c[:d0] + c[d1:]
if '> [!TIP]' in c:
    t0 = c.index('> [!TIP]')
    t1 = c.index('\n\n', t0)
    c = c[:t0] + c[t1 + 2:]
s = s[:i] + c + s[j:]
open(p, 'w', encoding='utf-8', newline='\n').write(s)
print('ok card', n)
