// Card 2: Ravi ko mera likha refresh pe hi dikha. Ilaaj: WebSocket (khula, dono taraf). Agar: connection toota -> version bata ke baaki op.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: HTTP ektarfa hai. Server khud Ravi ko nahi bhej sakta, Ravi ko refresh pe hi dikha'],
  [3, GRN, 'ILAAJ', 'ILAAJ: connection khula rehta, dono taraf (WebSocket). Server khud bhejta, ~200 ms me dikhta'],
  [6, YEL, 'AGAR', 'AGAR WebSocket toot gaya? -> judte hi "main v12 pe tha", server v13 se aage sab bhejta'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Main → Server', 'Maine "X" likha, HTTP request se server tak pahuncha'],
   [C.bad, 'Server → Ravi', 'HTTP ektarfa hai, server khud Ravi ko kuch nahi bhej sakta'],
   [C.acc, 'Ravi → Server', 'Ravi ne refresh dabaya, tab jaake use "X" dikha']],
  [[C.msg, 'Main → Server', 'Maine "X" likha, khule connection (WebSocket) se server pe gaya'],
   [C.good, 'Server → Ravi', 'Server ne khud Ravi ko bheja, ~200 ms me dikha, refresh nahi']],
  [[C.bad, 'Ravi ✕ Server', 'Ravi ka connection toota, us waqt uske paas v12 tha'],
   [C.msg, 'Main → Server', 'Main likhta raha, server pe doc v15 tak pahunch gaya'],
   [C.acc, 'Ravi → Server', 'Ravi dobara juda aur bola "main v12 pe tha"'],
   [C.good, 'Server → Ravi', 'Server ne v13, v14, v15 bheje, Ravi ka doc barabar']],
];
function wire(x1, x2, y) { el('line', { x1, y1: y, x2, y2: y, stroke: C.line, 'stroke-width': 10, 'stroke-linecap': 'round', opacity: 0.5 }, DYN); }
function frame(ph) {
  if (ph === 0) {
    B(15, 130, 170, 60, 'Main', 'likha: X', { icon: 'user' });
    B(360, 130, 180, 60, 'Server', 'X aa gaya', { icon: 'server' });
    B(710, 130, 175, 60, 'Ravi', 'purana: X nahi', { st: 'bad', icon: 'user' });
    A(185, 160, 358, 160, C.msg, { n: 1 }); L(272, 130, 'HTTP', C.msg);
    A(540, 155, 708, 155, C.bad, { n: 2, dash: 1, at: 0.3 }); cross(640, 155); L(625, 120, 'khud nahi bhej sakta', C.bad);
    AP([[797, 192], [797, 300], [450, 300], [450, 192]], C.acc, { n: 3, at: 0.3 }); L(600, 325, 'refresh dabaya, tab X dikha', C.acc);
  } else if (ph === 1) {
    wire(185, 360, 160); wire(540, 710, 160);
    B(15, 130, 170, 60, 'Main', 'likha: X', { icon: 'user' });
    B(360, 130, 180, 60, 'Server', 'WebSocket', { st: 'good', icon: 'server' });
    B(710, 130, 175, 60, 'Ravi', 'X dikha', { st: 'good', icon: 'user' });
    A(190, 140, 355, 140, C.msg, { n: 1 }); A(545, 140, 705, 140, C.good, { n: 2 });
    L(450, 230, 'connection khula rehta, dono taraf', C.muted); L(625, 185, '~200 ms', C.good);
    L(450, 300, 'Ravi ko refresh nahi karna padta', C.good);
  } else {
    B(15, 40, 170, 60, 'Main', 'likhta raha', { icon: 'user' });
    B(330, 130, 240, 95, 'Server', '', { st: 'good', icon: 'server' });
    cells(388, 185, [['v12'], ['v13', C.acc], ['v14', C.acc], ['v15', C.acc]], { w: 44, h: 30 });
    B(710, 130, 175, 60, 'Ravi', 'mere paas v12', { icon: 'user' });
    A(185, 70, 340, 140, C.msg, { n: 2 });
    A(705, 145, 575, 145, C.bad, { n: 1, dash: 1, at: 0.4 }); cross(620, 120); L(640, 95, 'connection toota', C.bad);
    A(705, 175, 575, 175, C.acc, { n: 3, at: 0.6 }); L(690, 250, '"main v12 pe tha"', C.acc);
    AP([[450, 227], [450, 300], [797, 300], [797, 192]], C.good, { n: 4, at: 0.3 }); L(620, 330, 'v13, v14, v15 bheje: barabar', C.good);
  }
}
