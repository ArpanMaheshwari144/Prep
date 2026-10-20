# Har card ki text kahani. Badlo -> python 04_HLD/tools/seqtxt.py <md> <ye file>
CARDS = {
1: [
  ('DIKKAT: har click seedha DB pe', ['User', 'App', 'DB'], [
    ('a', 0, 1, '1. click'),
    ('a', 1, 2, '2. padho', 'lakhon click / sec'),
    ('x', 2, '3. DB baith gaya'),
    ('a', 1, 0, '4. error'),
  ]),
  ('ILAAJ: beech me Redis (cache-aside)', ['User', 'App', 'Redis', 'DB'], [
    ('a', 0, 1, '1. click'),
    ('a', 1, 2, '2. hai?'),
    ('a', 2, 1, '3. mila', '95% click yahin khatam'),
    ('a', 1, 0, '4. link'),
    ('t', 1, 'nahi mila to:'),
    ('a', 1, 3, '5. DB se laao'),
    ('a', 1, 2, '6. copy rakho'),
  ]),
],
2: [
  ('DIKKAT: ek hi App machine', ['User', 'App', 'DB'], [
    ('a', 0, 1, '1. click'),
    ('x', 1, '2. App machine giri'),
    ('t', 0, '3. koi jawab nahi, poori site band'),
  ]),
  ('ILAAJ: kai App, aage LB (App kuch yaad nahi rakhta)', ['User', 'LB', 'App-1', 'App-2'], [
    ('a', 0, 1, '1. click'),
    ('a', 1, 2, '2. App-1 ko'),
    ('x', 2, '3. App-1 giri'),
    ('a', 1, 3, '4. agla click App-2 ko'),
    ('a', 3, 0, '5. link mila, site chalu'),
  ]),
],
3: [
  ('DIKKAT: dono App ne DB se ek hi number liya', ['Main', 'App-1', 'DB', 'App-2', 'Ravi'], [
    ('a', 0, 1, '1. naya link'),
    ('a', 4, 3, '2. naya link'),
    ('a', 1, 2, '3. agla no.?'),
    ('a', 3, 2, '4. agla no.?'),
    ('a', 2, 1, '5. 501'),
    ('a', 2, 3, '6. 501', 'dono ko ek hi code'),
  ]),
  ('ILAAJ: Counter har App ko alag range deta, code kabhi same nahi', ['Main', 'App-1', 'Counter', 'App-2', 'Ravi'], [
    ('a', 2, 1, '1. 1-1000'),
    ('a', 2, 3, '2. 1001-2000'),
    ('a', 0, 1, '3. naya link'),
    ('a', 1, 0, '4. code = 1'),
    ('a', 4, 3, '5. naya link'),
    ('a', 3, 4, '6. code = 1001'),
  ]),
],
4: [
  ('DIKKAT: click ginti redirect ke raaste me', ['User', 'App', 'DB'], [
    ('a', 0, 1, '1. click'),
    ('a', 1, 2, '2. ginti +1'),
    ('t', 2, '3. likhai ka wait'),
    ('a', 1, 0, '4. redirect der se'),
  ]),
  ('ILAAJ: redirect turant, ginti Kafka se peeche', ['User', 'App', 'Kafka', 'Analytics'], [
    ('a', 0, 1, '1. click'),
    ('a', 1, 0, '2. turant 302'),
    ('a', 1, 2, '3. click hua'),
    ('a', 2, 3, '4. apni speed se', 'ginti yahan'),
  ]),
],
5: [
  ('DIKKAT: saare link ek DB machine me', ['App', 'DB'], [
    ('a', 0, 1, '1. har link yahin'),
    ('x', 1, '2. 90 TB, ek machine me nahi samaata'),
  ]),
  ('ILAAJ: code se shard (ring), har tukde ki 3 copy', ['App', 'DB-1', 'DB-2', 'DB-3'], [
    ('t', 0, '1. code ka hash, ring pe aage chalo'),
    ('a', 0, 2, '2. pehli machine = DB-2'),
    ('t', 3, '3. baaki code doosri machine pe'),
  ]),
],
6: [
  ('DIKKAT: ek copy me likha, doosri se padha', ['App', 'Copy A', 'Copy B', 'Copy C'], [
    ('a', 0, 1, '1. naya link'),
    ('a', 0, 3, '2. turant padha'),
    ('a', 3, 0, '3. nahi mila, 404', 'C abhi purani'),
  ]),
  ('ILAAJ: 2 me likho, 2 se padho (quorum)', ['App', 'Copy A', 'Copy B', 'Copy C'], [
    ('a', 0, 1, '1. likho'),
    ('a', 0, 2, '2. likho'),
    ('a', 0, 2, '3. padho'),
    ('a', 0, 3, '4. padho'),
    ('a', 2, 0, '5. B me mila', 'jodi me ek taaza zaroor'),
  ]),
],
7: [
  ('DIKKAT: ek hi LB', ['User', 'DNS', 'LB-1'], [
    ('a', 0, 1, '1. site kahan?'),
    ('a', 1, 0, '2. LB-1'),
    ('a', 0, 2, '3. request'),
    ('x', 2, '4. LB-1 gira, koi andar nahi'),
  ]),
  ('ILAAJ: do LB, aage Route 53 (health check)', ['User', 'Route 53', 'LB-1', 'LB-2'], [
    ('a', 1, 2, '1. /health? (~30s)'),
    ('x', 2, '2. 3 baar chup'),
    ('t', 1, '3. LB-1 list se bahar'),
    ('a', 0, 1, '4. site kahan?'),
    ('a', 1, 0, '5. LB-2'),
    ('a', 0, 3, '6. request andar'),
  ]),
],
8: [
  ('DIKKAT: koi had nahi', ['Spammer', 'App', 'DB'], [
    ('a', 0, 1, '1. 5,000 / sec'),
    ('a', 1, 2, '2. sab likho'),
    ('x', 2, '3. DB bhara, sab dheema'),
  ]),
  ('ILAAJ: gateway pe ginti (rate limit)', ['Spammer', 'Gateway', 'Redis', 'App'], [
    ('a', 0, 1, '1. link banao'),
    ('a', 1, 2, '2. key ginti +1'),
    ('a', 2, 1, '3. had paar'),
    ('a', 1, 0, '4. 429', 'App tak aaya hi nahi'),
  ]),
],
}
