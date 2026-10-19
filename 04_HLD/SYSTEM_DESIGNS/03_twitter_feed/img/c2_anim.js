// Card 2: usi request me 5,000 inbox bharo -> button 5 sec ghooma. Ilaaj: DB save + Kafka, workers peeche.
const DUR = 16, FIX = 7.5;
const ME = [70, 192], APP = [200, 205], K = [450, 290], W = [640, 290], IN = [800, 192];
function setup() {
  const fix = grp('fix');
  box(20, 150, 100, 84, 'Main', '5,000 follower');
  wire('M120 192 H200'); wire('M300 192 H720');
  box(200, 165, 100, 54, 'Tweet Svc');
  box(720, 150, 140, 84, '5,000 inbox', '');
  box(400, 50, 100, 50, 'Tweet DB', '', { parent: fix }); wire('M250 165 L400 80', '', fix);
  box(400, 265, 100, 50, 'Kafka', '', { parent: fix }); wire('M250 219 L400 290', '', fix);
  box(590, 265, 100, 50, 'Workers', '', { parent: fix }); wire('M500 290 H590', '', fix); wire('M690 290 L760 234', '', fix);
}
function frame(t, fix, tl) {
  vis('fix', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: usi request me 5,000 inbox ek-ek karke. 5+ sec, timeout, mujhe "fail" dikha';
    dot(path([[0.3, ME], [0.8, APP]], tl), C.msg);
    const f = Math.min(1, Math.max(0, (tl - 0.8) / 5));
    for (let i = 0; i < 4; i++) { const s = 0.9 + i * 0.4; if (tl > s) dot(path([[s, [300, 192]], [s + 0.8, [720, 192]]], (tl - s) % 1.2 + s), C.acc); }
    bar(730, 245, 120, 14, f, '');
    T(790, 280, `bhare: ${Math.round(5000 * f)} / 5000`, { c: C.fg, size: 12 });
    const sec = Math.min(5, (tl - 0.8)).toFixed(1);
    if (tl > 0.8) T(20, 130, tl < 6 ? `button ghoom raha ${sec}s` : 'timeout: "fail"', { c: tl < 6 ? C.acc : C.bad, b: 1, size: 13, a: 'start' });
    if (tl > 6) T(250, 260, 'par tweet ban chuka tha!', { c: C.bad, size: 12, b: 1 });
    return;
  }
  CAP.textContent = 'ILAAJ: tweet DB me save + khabar Kafka me, mujhe turant "ho gaya". Workers peeche inbox bharte';
  dot(path([[0.3, ME], [0.8, APP]], tl < 1.0 ? tl : -1), C.msg);
  if (tl > 0.8) { dot(path([[0.8, APP], [1.3, [400, 80]]], tl < 1.3 ? tl : -1), C.good);
                  dot(path([[0.8, APP], [1.3, K]], tl < 1.3 ? tl : -1), C.acc); }
  if (tl > 1.3) T(20, 130, '"ho gaya" 50 ms me', { c: C.good, b: 1, size: 13, a: 'start' });
  const f = Math.min(1, Math.max(0, (tl - 1.6) / 4));
  if (tl > 1.6 && f < 1) dot(path([[0, K], [0.5, W], [1.0, IN]], (tl - 1.6) % 1.0), C.acc);
  bar(730, 245, 120, 14, f, '', true);
  T(790, 280, `peeche bhare: ${Math.round(5000 * f)}`, { c: C.good, size: 12 });
}
