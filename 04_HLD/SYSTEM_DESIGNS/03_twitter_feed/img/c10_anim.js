// Card 10: 50 MB ki video, sabko US server se, dheemi aur mehngi. Ilaaj: S3 + CDN, upload seedha S3 (presigned).
// Agar: private photo -> link pe hastakshar + expiry (signed URL).
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 50 MB ki video. Delhi, London, Sydney, sab hamare US server se kheenchte'],
  [3, GRN, 'ILAAJ', 'ILAAJ: video S3 me (seedha upload), dekhne wale ko paas wale CDN se'],
  [6, YEL, 'AGAR', 'AGAR private photo hai, link kisi aur ko mil gaya? -> link pe hastakshar + expiry'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'US server → Delhi, London, Sydney', '50 MB ki video laakhon logon ko door US server se ja rahi'], [C.bad, 'US server', 'Bandwidth ka bill bhaari, aur door walon ko buffering']],
  [[C.msg, 'Arpan → App', 'Arpan ne App se upload link maanga, presigned link mila'], [C.good, 'Arpan → S3', 'Video seedha S3 me gayi, App beech me bojh nahi utha raha'], [C.msg, 'Delhi → CDN Delhi', 'Delhi ke user ne video dekhni chahi, request paas wale CDN pe'], [C.muted, 'S3 → CDN Delhi', 'Pehli baar CDN ne S3 se laayi, phir paas se hi deta']],
  [[C.good, 'Mohan → CDN', 'Mohan ke paas signed link hai, 10 min ki umar wala'], [C.good, 'CDN → Mohan', 'Link sahi aur zinda tha, to Mohan ko photo mil gayi'], [C.bad, 'Koi aur → CDN', 'Kisi aur ne 1 ghante baad wahi link khola'], [C.bad, 'CDN → Koi aur', 'Link ki umar khatam, to CDN ne 403 diya']],
];
function frame(ph) {
  if (ph === 0) {
    ['Delhi', 'London', 'Sydney'].forEach((c, i) => {
      B(20, 30 + i * 105, 130, 60, c, 'buffering...', { st: 'bad' });
      A(658, 160, 152, 60 + i * 105, C.bad, { n: i ? 0 : 1, at: 0.3 });
    });
    B(660, 120, 200, 80, 'US server', '50 MB x laakhon', { st: 'bad' });
    num(760, 240, 2, C.bad); L(760, 265, 'bandwidth ka bill', C.bad);
    L(450, 345, 'har dekhne wala door se, hamari machine se', C.bad);
  } else if (ph === 1) {
    B(20, 30, 130, 60, 'Arpan', 'video daali');
    B(260, 30, 150, 60, 'App', 'upload link');
    B(560, 30, 200, 60, 'S3', 'video yahan', { st: 'good' });
    A(150, 50, 258, 50, C.msg, { n: 1 }); A(258, 78, 152, 78, C.acc); L(205, 110, 'presigned link', C.acc);
    AP([[85, 92], [85, 140], [660, 140], [660, 92]], C.good, { n: 2, at: 0.5 }); L(380, 160, 'seedha S3, App beech me nahi', C.good);
    B(20, 240, 130, 60, 'Delhi', 'dekhna');
    B(330, 240, 170, 60, 'CDN Delhi', 'paas me', { st: 'good' });
    A(150, 270, 328, 270, C.msg, { n: 3 }); A(620, 92, 500, 255, C.muted, { n: 4, dash: 1 }); L(560, 220, 'pehli baar S3 se laaya', C.muted, 'start');
    L(450, 340, 'baaki Delhi wale CDN Delhi se: paas bhi, sasta bhi', C.good);
  } else {
    B(20, 60, 130, 60, 'Mohan', 'follower');
    B(20, 240, 130, 60, 'Koi aur', 'link mila');
    B(360, 140, 220, 70, 'CDN (private photo)', 'hastakshar check', { st: 'acc' });
    A(150, 100, 358, 175, C.good, { n: 1 }); L(225, 160, 'signed link, 10 min', C.good);
    A(358, 150, 152, 70, C.good, { n: 2 }); L(290, 85, 'photo mili', C.good);
    A(150, 248, 358, 200, C.bad, { n: 3 }); L(250, 205, '1 ghante baad', C.bad);
    A(370, 212, 152, 292, C.bad, { n: 4, at: 0.6 }); L(330, 300, '403: link ki umar khatam', C.bad, 'start');
    L(450, 345, 'sirf jisko mila, aur sirf thodi der ke liye', C.good);
  }
}
