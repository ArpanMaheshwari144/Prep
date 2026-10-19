// Chalti tasveer ka base. Scene file (cN_anim.js) deta hai: DUR, FIX, setup(), frame(t, fix, tl).
// anim_build.py ise + scene ko ek html me jodta hai, phir anim2gif.js GIF banata hai.
const NS = 'http://www.w3.org/2000/svg';
const C = { bg: '#11161c', box: '#222d3a', line: '#4b5a6a', fg: '#e6edf3', muted: '#9aa8b6',
  msg: '#64b5f6', good: '#66bb6a', bad: '#ef5350', acc: '#f0a030' };
const ST = document.getElementById('st'), DYN = document.getElementById('dyn');
function el(tag, a, parent, text) {
  const e = document.createElementNS(NS, tag);
  for (const k in a) e.setAttribute(k, a[k]);
  if (text !== undefined) e.textContent = text;
  (parent || ST).append(e); return e;
}
function grp(id) { return el('g', id ? { id } : {}); }
// dabba: label upar ki taraf, taaki beech se dot guzre to naam na dhake
function box(x, y, w, h, label, sub, o = {}) {
  const g = el('g', o.id ? { id: o.id } : {}, o.parent);
  el('rect', { x, y, width: w, height: h, rx: o.r || 8, fill: o.fill || C.box, stroke: o.stroke || C.line,
    'stroke-width': 1.5 }, g);
  el('text', { x: x + w / 2, y: y + (sub || h >= 60 ? 20 : h / 2 + 5), 'text-anchor': 'middle', 'font-size': 14, fill: C.fg }, g, label);
  if (sub) el('text', { x: x + w / 2, y: y + 36, 'text-anchor': 'middle', 'font-size': 12, fill: C.muted }, g, sub);
  return g;
}
function wire(d, id, parent) {
  return el('path', { d, id: id || '', fill: 'none', stroke: C.line, 'stroke-width': 1.5, 'stroke-dasharray': '4 4' }, parent);
}
function vis(id, on) { const e = document.getElementById(id); if (e) e.style.opacity = on ? 1 : 0; }
// har frame DYN saaf, phir naya
function T(x, y, s, o = {}) {
  return el('text', { x, y, 'text-anchor': o.a || 'middle', 'font-size': o.size || 13, fill: o.c || C.fg,
    'font-weight': o.b ? 'bold' : 'normal', 'paint-order': 'stroke', stroke: C.bg, 'stroke-width': o.halo ? 5 : 0 }, DYN, s);
}
function dot(p, c, num, label, lc) {
  if (!p) return;
  el('circle', { cx: p[0], cy: p[1], r: 9, fill: c || C.msg }, DYN);
  if (num !== undefined && num !== '') el('text', { x: p[0], y: p[1] + 4, 'text-anchor': 'middle', 'font-size': 10,
    'font-weight': 'bold', fill: C.bg }, DYN, num);
  if (label) T(p[0], p[1] - 15, label, { c: lc || c || C.msg, b: 1, size: 12, halo: 1 });
}
function cross(x, y) {
  el('path', { d: `M${x - 14} ${y - 14} L${x + 14} ${y + 14} M${x + 14} ${y - 14} L${x - 14} ${y + 14}`,
    stroke: C.bad, 'stroke-width': 4 }, DYN);
}
function lerp(a, b, f) { return [a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f]; }
// pts: [[time, [x,y]], ...] ; pehle se pehle null, aakhir ke baad aakhri jagah
function path(pts, t) {
  if (t < pts[0][0]) return null;
  for (let i = 1; i < pts.length; i++) if (t <= pts[i][0])
    return lerp(pts[i - 1][1], pts[i][1], (t - pts[i - 1][0]) / (pts[i][0] - pts[i - 1][0]));
  return pts[pts.length - 1][1];
}
function bar(x, y, w, h, frac, label) {
  el('rect', { x, y, width: w, height: h, rx: 4, fill: 'none', stroke: C.line }, DYN);
  const c = frac > 0.9 ? C.bad : frac > 0.6 ? C.acc : C.good;
  el('rect', { x: x + 2, y: y + 2, width: Math.max(0, (w - 4) * Math.min(1, frac)), height: h - 4, rx: 3, fill: c }, DYN);
  if (label) T(x + w / 2, y - 6, label, { size: 12, c: C.muted });
}
const CAP = document.getElementById('cap'), CAPBG = document.getElementById('capbg');
function draw(t) {
  DYN.replaceChildren();
  const fix = t >= FIX;
  CAPBG.setAttribute('fill', fix ? '#123018' : '#3a1414');
  frame(t, fix, fix ? t - FIX : t);
}
