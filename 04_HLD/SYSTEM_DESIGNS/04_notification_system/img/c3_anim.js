// Card 3: Worker-A ne email bheja, "padh liya" bolne se pehle mara, Worker-B ne phir bheja -> 2 email. Ilaaj: khabar_id + raasta ki key Redis me.
const DUR = 18, FIX = 8;
const K = [140, 190], A = [290, 85], B = [290, 275], AR = [430, 85], BR = [430, 275];
const mv = (a, b, s, e, tl) => (tl >= s && tl <= e ? path([[s, a], [e, b]], tl) : null);
function setup() {
  box(20, 150, 120, 80, 'Kafka', 'khabar K-1');
  wire('M140 190 L290 85'); wire('M140 190 L290 275');
  box(290, 50, 140, 70, 'Worker-A'); box(290, 240, 140, 70, 'Worker-B');
  wire('M430 85 L730 170'); wire('M430 275 L730 200');
  box(730, 150, 130, 80, 'User', 'email inbox');
  wire('M430 85 L510 165', 'wa'); wire('M430 275 L510 205', 'wb');
  box(510, 150, 140, 70, 'Redis', 'khabar_id+raasta', { id: 'rd', stroke: C.acc });
}
function frame(t, fix, tl) {
  vis('rd', fix); vis('wa', fix); vis('wb', fix);
  let mails = 0;
  dot(mv(K, A, 0.3, 0.9, tl), C.msg);
  const crash = () => {
    if (tl > 2.4 && tl < 3.4) {
      el('rect', { x: 290, y: 50, width: 140, height: 70, rx: 8, fill: C.bad, opacity: 0.35 }, DYN);
      T(360, 38, 'mara, "padh liya" nahi bola', { c: C.bad, b: 1, size: 12 });
    }
    if (tl >= 3.4) { cross(450, 60); T(140, 135, 'khabar dobara', { c: C.acc, b: 1, size: 12 }); }
  };
  if (!fix) {
    CAP.textContent = 'DIKKAT: Worker-A ne email bheja, "padh liya" bolne se pehle mara. Worker-B ne phir bheja';
    dot(mv(AR, [718, 175], 1.1, 2.0, tl), C.msg); if (tl >= 2.0) mails++;
    crash();
    dot(mv(K, B, 3.6, 4.2, tl), C.msg);
    dot(mv(BR, [718, 195], 4.4, 5.3, tl), C.bad); if (tl >= 5.3) mails++;
    T(795, 255, `email: ${mails}`, { c: mails > 1 ? C.bad : C.fg, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: bhejne se pehle key (khabar_id + raasta) Redis me. Mili to skip, ek email';
  dot(mv(AR, [498, 165], 1.0, 1.4, tl), C.acc, '', 'K-1:email?', C.acc);
  if (tl > 1.4) T(580, 245, 'K-1:email likha', { c: C.acc, b: 1, size: 12, halo: 1 });
  dot(mv(AR, [718, 175], 1.6, 2.4, tl), C.good); if (tl >= 2.4) mails++;
  crash();
  dot(mv(K, B, 3.6, 4.2, tl), C.msg);
  dot(mv(BR, [498, 205], 4.4, 4.9, tl), C.acc, '', 'K-1:email?', C.acc);
  if (tl > 5.0) T(440, 335, 'key hai -> skip', { c: C.good, b: 1, size: 14 });
  T(795, 255, `email: ${mails}`, { c: C.good, b: 1, size: 15 });
}
