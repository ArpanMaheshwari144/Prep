// Card 7: email bheja, commit se pehle crash -> restart pe phir offset 5 -> 2 email. Ilaaj: eventId Redis me, dobara aaye to skip.
const DUR = 18, FIX = 8;
function setup() {
  box(40, 150, 140, 84, 'Kafka', 'offset 5 = order');
  wire('M180 192 H330'); wire('M470 192 H700');
  box(330, 155, 140, 74, 'Email consumer');
  box(700, 155, 120, 74, 'Ravi');
  box(330, 285, 140, 60, 'Redis', 'eventId', { id: 'rd', stroke: C.acc }); wire('M400 229 V285', 'wr');
}
function frame(t, fix, tl) {
  vis('rd', fix); vis('wr', fix);
  let mails = 0;
  const pass = (s, col) => { dot(path([[s, [180, 192]], [s + 0.6, [330, 205]]], tl < s + 0.6 ? tl : -1), col);
    dot(path([[s + 0.8, [470, 205]], [s + 1.4, [700, 205]]], tl > s + 0.8 && tl < s + 1.4 ? tl : -1), col);
    return tl >= s + 1.4; };
  const crash = () => { if (tl > 2.0 && tl < 3.0) { el('rect', { x: 330, y: 155, width: 140, height: 74, rx: 8, fill: C.bad, opacity: 0.35 }, DYN);
    T(400, 140, 'crash, commit nahi hua', { c: C.bad, b: 1, size: 12 }); }
    if (tl > 3.0) T(400, 140, 'restart: phir offset 5', { c: C.acc, b: 1, size: 12 }); };
  if (!fix) {
    CAP.textContent = 'DIKKAT: email chala gaya, "padh liya" (commit) bolne se pehle crash. Restart pe phir 5 se';
    if (pass(0.4, C.msg)) mails++;
    crash();
    if (pass(3.2, C.msg)) mails++;
    T(760, 260, `email: ${mails}`, { c: mails > 1 ? C.bad : C.fg, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: pehle kaam, phir commit. Har khabar ka eventId Redis me: dobara aaye to skip';
  if (pass(0.4, C.good)) mails++;
  if (tl > 1.0 && tl < 1.6) dot(path([[1.0, [400, 229]], [1.6, [400, 285]]], tl), C.acc);
  if (tl > 1.6) T(490, 320, 'E-5 likha hua', { c: C.acc, size: 12, b: 1, a: 'start' });
  crash();
  dot(path([[3.2, [180, 192]], [3.8, [330, 205]]], tl < 3.8 ? tl : -1), C.msg);
  if (tl > 3.8 && tl < 4.4) dot(path([[3.8, [400, 229]], [4.4, [400, 285]]], tl), C.acc, '', 'E-5?', C.acc);
  if (tl > 4.4) T(500, 270, 'pehle se hai -> skip', { c: C.good, b: 1, a: 'start', size: 13 });
  T(760, 260, `email: ${mails}`, { c: C.good, b: 1, size: 15 });
}
