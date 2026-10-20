// Card 10: har aana-jaana saare contacts ko -> bahut message. Ilaaj: online = Redis key + umar 30 sec, dhadkan 15 sec.
const DUR = 19, FIX = 8;
const RV = [150, 190], SL = [218, 190], SR = [360, 190], RDL = [438, 75], AR = [688, 260];
function setup() {
  wire('M150 190 H230');
  box(20, 150, 130, 80, 'Ravi ka app');
  box(230, 150, 130, 80, 'Chat Server');
  wire('M360 190 L600 190', 'wc');
  box(600, 20, 280, 320, '500 contacts', '', { id: 'ct' });
  wire('M360 170 L450 90', 'wr'); wire('M560 120 L700 270', 'wa');
  box(450, 30, 220, 90, 'Redis', 'online:Ravi', { id: 'rd', stroke: C.acc });
  box(700, 230, 180, 90, 'Arpan', 'Ravi ki chat khuli', { id: 'ap' });
}
function frame(t, fix, tl) {
  vis('ct', !fix); vis('wc', !fix);
  ['wr', 'wa', 'rd', 'ap'].forEach(i => vis(i, fix));
  if (!fix) {
    CAP.textContent = 'DIKKAT: har aana-jaana saare contacts ko: 500 contacts x crore log, bahut message';
    const ev = [[0.4, 'online'], [2.6, 'offline'], [4.8, 'online']];
    let sent = 0;
    ev.forEach(([s, lab]) => {
      if (tl > s && tl < s + 0.6) dot(path([[s, RV], [s + 0.6, SL]], tl), C.msg, '', lab);
      for (let k = 0; k < 12; k++) {
        const s2 = s + 0.6 + k * 0.05;
        if (tl > s2 && tl < s2 + 0.8) dot(path([[s2, SR], [s2 + 0.8, [620 + (k % 4) * 70, 70 + Math.floor(k / 4) * 90]]], tl), C.bad);
      }
      if (tl > s + 1.4) sent += 500;
    });
    for (let i = 0; i < 40; i++)
      el('circle', { cx: 625 + (i % 8) * 33, cy: 75 + Math.floor(i / 8) * 50, r: 6, fill: 'none', stroke: C.line }, DYN);
    T(740, 325, `ek Ravi se: ${sent} message`, { c: C.bad, b: 1, size: 13 });
    if (tl > 6.4) T(300, 300, 'x crore log = bahut message', { c: C.bad, b: 1, size: 14 });
    return;
  }
  // 1 sec asal = 6 sec kahani me
  CAP.textContent = 'ILAAJ: online = Redis key + umar (30 sec), app har 15 sec dhadkan. Status tabhi jab chat khule';
  const hb = [0.5, 3.0];
  let last = -1;
  hb.forEach(s => {
    if (tl > s && tl < s + 0.5) dot(path([[s, RV], [s + 0.5, SL]], tl), C.good, '', 'hoon');
    if (tl > s + 0.5 && tl < s + 1.0) dot(path([[s + 0.5, SR], [s + 1.0, RDL]], tl), C.good);
    if (tl > s + 1.0) last = s + 1.0;
  });
  const umar = last < 0 ? null : Math.max(0, Math.round(30 - (tl - last) * 6));
  const on = umar !== null && umar > 0;
  if (umar !== null) T(560, 100, on ? `umar: ${umar} sec` : 'key khud mitti', { c: on ? C.good : C.bad, b: 1, size: 13 });
  if (tl > 4.2) T(85, 260, 'app band: dhadkan band', { c: C.bad, b: 1, size: 12 });
  const ask = (s, ans, c) => {
    if (tl > s && tl < s + 0.6) dot(path([[s, AR], [s + 0.6, [600, 132]]], tl), C.acc, '', 'Ravi online?', C.acc);
    if (tl > s + 0.6 && tl < s + 1.2) dot(path([[s + 0.6, [600, 132]], [s + 1.2, AR]], tl), c, '', ans, c);
    if (tl > s + 1.2) T(790, 300, `Ravi: ${ans}`, { c, b: 1, size: 13 });
  };
  if (tl < 7.6) ask(2.0, 'online', C.good); else ask(8.6, 'offline', C.bad);
  if (tl > 7.4) T(230, 330, 'maango tab dekho, sabko batana nahi', { a: 'start', c: C.good, size: 12 });
}
