# SQL Window Functions

MySQL 5.7 pe ye chalta hi nahi: `ROW_NUMBER() OVER (...)` pe `ERROR 1064 syntax error`, kyunki window function MySQL 8 se aaye. Neeche ke
saare output SQLite pe chala ke nikaale hain (9-Nov).

## Data

```
orders:  Arpan day1 100 · Arpan day2 200 · Arpan day3 300 · Ravi day1 50 · Ravi day2 150
```

## GROUP BY ya window?

GROUP BY rows ko daba ke ek kar deta hai:
```
SELECT customer, SUM(amount) FROM orders GROUP BY customer;
| Arpan | 600 |
| Ravi  | 200 |
```
Window wahi hisaab karta hai, par har row apni jagah rehti hai:
```
SUM(amount) OVER (PARTITION BY customer) AS customer_total
| Arpan | 1 | 100 | 600 |
| Arpan | 2 | 200 | 600 |
| Arpan | 3 | 300 | 600 |
| Ravi  | 1 |  50 | 200 |
| Ravi  | 2 | 150 | 200 |
```
`PARTITION BY` = kis hisaab se baanto (GROUP BY jaisa, par row dabti nahi).

## Running total (prefix sum)

`ORDER BY` lagaya to har row tak ka jod:
```
SUM(amount) OVER (PARTITION BY customer ORDER BY day) AS running
| Arpan | 1 | 100 | 100 |
| Arpan | 2 | 200 | 300 |
| Arpan | 3 | 300 | 600 |
| Ravi  | 1 |  50 |  50 |      <- Ravi pe naya shuru
| Ravi  | 2 | 150 | 200 |
```

## Moving average (sliding window)

Pichhli 1 row + ye wali:
```
AVG(amount) OVER (PARTITION BY customer ORDER BY day ROWS BETWEEN 1 PRECEDING AND CURRENT ROW)
| Arpan | 1 | 100 | 100 |
| Arpan | 2 | 200 | 150 |      <- (100+200)/2
| Arpan | 3 | 300 | 250 |      <- (200+300)/2
```
`ROWS` = ginti se khidki (pichhli N row). `RANGE` = value se khidki (jaise pichhle 1 minute ki saari row:
MySQL 8 me `RANGE BETWEEN INTERVAL 1 MINUTE PRECEDING AND CURRENT ROW`, Postgres me `INTERVAL '1 minute'`).

## Rank wale teen

Backend: Arpan 90, Ravi 80, Sita 80, Mehta 70
```
name   salary  ROW_NUMBER  RANK  DENSE_RANK
Arpan  90      1           1     1
Ravi   80      2           2     2
Sita   80      3           2     2      <- barabar
Mehta  70      4           4     3      <- RANK ne 3 chhoda, DENSE_RANK ne nahi
```

## Classic sawaal: har department me 2nd highest salary

```sql
SELECT * FROM (
    SELECT name, dept, salary,
           DENSE_RANK() OVER (PARTITION BY dept ORDER BY salary DESC) AS rnk
    FROM emp
) t
WHERE rnk = 2;
```
```
| Ravi | Backend  | 80 |
| Sita | Backend  | 80 |      <- dono 80, dono 2nd
| Raj  | Frontend | 50 |
```
Window function ko WHERE me seedha nahi likh sakte, kyunki WHERE pehle chalta hai. Isliye andar subquery me lapeto.

## LAG / LEAD

"Kal se kitna badha?":
```
amount - LAG(amount) OVER (PARTITION BY customer ORDER BY day)
| Arpan | 1 | 100 | NULL |      <- pehle din pichhla hai hi nahi
| Arpan | 2 | 200 | 100  |
| Arpan | 3 | 300 | 100  |
```
`LEAD` = agli row ki value.

## Trap

"2nd highest" me `ROW_NUMBER` lagaya to Ravi aur Sita (dono 80) me se sirf ek aata, aur kaun aayega ye tay nahi. `RANK` lagaya to barabari ke
baad wala number chhoot jaata hai. "N-th highest" me **DENSE_RANK** hi lo.
