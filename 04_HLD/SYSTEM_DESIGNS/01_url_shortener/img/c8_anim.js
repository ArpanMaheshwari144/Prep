// Card 8: spammer 5000 link/sec. Ilaaj: gateway pe rate limit. Agar: pehchaana hua hamla -> WAF.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: koi had nahi, spammer ke 5,000 link / sec seedhe DB tak, sab dheema'],
  [3, GRN, 'ILAAJ', 'ILAAJ: gateway pe har API key ki ek ginti (Redis). Had paar to 429, andar nahi'],
  [6, YEL, 'AGAR', 'AGAR darwaze pe pehchaana hua hamla aaya? -> WAF darwaze pe hi rok deta'],
];
function setup() {}
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 130, 60, 'Spammer', '5,000 / sec', { st: 'bad' });
    A(150, 160, 318, 160, C.bad, { n: 1, at: 0.7 }); dots(5, 170, 140, C.bad, { gap: 18 });
    B(320, 130, 120, 60, 'App'); A(440, 160, 598, 160, C.bad, { n: 2, at: 0.7 }); dots(5, 460, 140, C.bad, { gap: 18 });
    B(600, 130, 120, 60, 'DB', '', { st: 'bad' }); cross(745, 160); bar(600, 215, 120, 16, 1, ''); L(660, 255, 'bhara, sab dheema', C.bad);
    B(20, 260, 130, 50, 'Asli user'); A(150, 285, 318, 185, C.muted, { dash: 1 }); L(240, 300, 'inka bhi dheema', C.muted);
  } else if (ph === 1) {
    B(20, 130, 130, 60, 'Spammer', 'key: k1', { st: 'bad' });
    A(150, 160, 318, 160, C.bad, { n: 1 });
    B(320, 130, 130, 60, 'Gateway', 'rate limit', { st: 'acc' });
    B(540, 30, 130, 60, 'Redis', 'k1: 5000 / 100', { st: 'acc' });
    A(440, 130, 530, 62, C.acc, { n: 2, at: 0.35 }); L(440, 85, 'ginti +1', C.acc);
    A(560, 92, 470, 150, C.bad, { n: 3, at: 0.45 }); L(560, 135, 'had paar', C.bad, 'start');
    A(318, 180, 152, 180, C.bad, { n: 4 }); T(185, 210, '429', { c: C.bad, b: 1, size: 18 });
    B(700, 130, 120, 60, 'App', '', { st: 'good' }); A(450, 170, 698, 170, C.muted, { dash: 1 }); cross(575, 170); L(575, 215, 'k1 App tak aaya hi nahi', C.bad);
    B(20, 260, 130, 50, 'Asli user', 'key: k2'); A(150, 285, 318, 185, C.good, { n: 5 });
    // asli user had ke andar -> Gateway use App tak jaane deta
    AP([[400, 192], [400, 260], [760, 260], [760, 192]], C.good, { n: 6, at: 0.5 }); L(580, 285, 'k2 had ke andar, App tak, speed bachi', C.good);
  } else {
    B(20, 130, 130, 60, 'Attacker', 'SQL inject / bot', { st: 'bad' });
    A(150, 160, 238, 160, C.bad, { n: 1 });
    B(240, 130, 110, 60, 'WAF', 'darwaza', { st: 'acc' }); cross(385, 160); L(295, 215, 'pehchaana hua, roka', C.bad);
    A(350, 160, 478, 160, C.muted, { dash: 1 });
    B(480, 130, 130, 60, 'Gateway', 'rate limit'); A(610, 160, 698, 160, C.muted, { dash: 1 }); B(700, 130, 120, 60, 'App');
    B(20, 260, 130, 50, 'Asli user'); A(150, 285, 238, 190, C.good, { n: 2 }); A(350, 175, 478, 175, C.good); A(610, 175, 698, 175, C.good);
    L(540, 300, 'WAF = pehchaane hue hamle, rate limit = bheed. Dono darwaze pe', C.muted);
  }
}
