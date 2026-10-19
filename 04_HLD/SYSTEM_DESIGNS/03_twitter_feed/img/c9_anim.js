// Card 9: padhna + likhna ek service me, highlights ki bheed me mera tweet dheema. Ilaaj: alag service, ALB path se.
const DUR = 17, FIX = 8;
const R = [70, 110], W = [70, 280], LB = [260, 192];
function setup() {
  const bug = grp('bug'), fix = grp('fix');
  box(20, 70, 100, 80, 'Crore log', 'GET /feed');
  box(20, 245, 100, 70, 'Main', 'POST /tweet');
  box(210, 165, 100, 54, 'ALB');
  wire('M120 110 L210 180'); wire('M120 280 L210 205');
  box(470, 150, 150, 84, 'Ek service', 'padhna + likhna', { parent: bug }); wire('M310 192 H470', '', bug);
  box(470, 50, 170, 70, 'Timeline Svc x 20', 'GET /feed', { parent: fix }); wire('M310 185 L470 85', '', fix);
  box(470, 260, 170, 70, 'Tweet Svc x 2', 'POST /tweet', { parent: fix, stroke: C.acc }); wire('M310 200 L470 295', '', fix);
}
function frame(t, fix, tl) {
  vis('bug', !fix); vis('fix', fix);
  for (let k = 0; k < 16; k++) { const s = 0.2 + k * 0.25;
    const end = fix ? [470, 85] : [470, 192];
    const p = path([[s, R], [s + 0.4, LB], [s + 0.9, end]], tl);
    if (tl < s + 1.0) dot(p, C.msg); }
  if (!fix) {
    CAP.textContent = 'DIKKAT: padhna aur likhna ek hi service me. Crore log scroll kar rahe, mera tweet usi bheed me';
    const p = path([[1.0, W], [1.5, LB], [2.0, [440, 205]], [5.5, [468, 205]]], tl);
    dot(p, C.acc, '', tl > 2.0 ? '' : 'mera tweet', C.acc);
    if (tl > 2.0) T(545, 255, 'mera tweet bheed ki line me', { c: C.acc, b: 1, size: 12 });
    if (tl > 2.5) T(545, 278, `ruka: ${Math.min(4, tl - 2).toFixed(1)} sec`, { c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: likhne aur padhne ki alag service. ALB path se baante. Jahan bheed, wahi badhao';
  const p = path([[1.0, W], [1.5, LB], [2.0, [470, 295]]], tl);
  if (tl < 2.4) dot(p, C.acc, '', 'mera tweet', C.acc);
  if (tl > 2.0) T(700, 300, 'turant save', { c: C.good, b: 1, size: 13, a: 'start' });
  T(700, 90, 'bheed sirf yahan', { c: C.muted, size: 12, a: 'start' });
}
