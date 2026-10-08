# SQL WINDOW FUNCTIONS — ek nazar (BlackRock + banks me aaya)

> Ek line: **GROUP BY jaisa hisaab, par har ROW wahin rehti hai.**
> Running total = tera PREFIX SUM · moving average = tera SLIDING WINDOW. Naya sirf syntax hai.

```
FUNC(...) OVER ( PARTITION BY x      <- khidki kaise baante (GROUP BY jaisa, row dabti nahi)
                 ORDER BY y          <- khidki ke andar kram
                 ROWS / RANGE ... )  <- khidki kitni badi (moving wale me)
```

## GROUP BY vs WINDOW
```
GROUP BY    SELECT customer, SUM(amount) FROM orders GROUP BY customer;
            -> A ki 3 row dab ke 1 row

WINDOW      SELECT id, customer, amount,
                   SUM(amount) OVER (PARTITION BY customer) AS customer_total
            FROM orders;
            -> A ki teeno row rahi, har row pe A ka total (600)
```

## RUNNING TOTAL (prefix sum)
```sql
SUM(amount) OVER (PARTITION BY customer ORDER BY day) AS running_total
```
```
A  day1 100 -> 100 · day2 200 -> 300 · day3 300 -> 600 · B pe naya shuru
```

## MOVING AVERAGE (sliding window)
```sql
-- last 2 ROWS ka average
AVG(amount) OVER (PARTITION BY customer ORDER BY day
                  ROWS BETWEEN 1 PRECEDING AND CURRENT ROW)

-- last 1 MINUTE ka average (time ke hisaab se khidki)
-- MySQL 8:
AVG(price) OVER (ORDER BY ts
                 RANGE BETWEEN INTERVAL 1 MINUTE PRECEDING AND CURRENT ROW)
-- Postgres (11+): interval quote me
AVG(price) OVER (ORDER BY ts
                 RANGE BETWEEN INTERVAL '1 minute' PRECEDING AND CURRENT ROW)
```
```
ROWS   = GINTI se khidki (pichhli N row)
RANGE  = VALUE se khidki (pichhle 1 minute me jitni bhi row)
```

## RANK wale teen
```
score   ROW_NUMBER   RANK   DENSE_RANK
90      1            1      1
80      2            2      2
80      3            2      2        <- barabar
70      4            4      3        <- RANK ne 3 chhoda, DENSE_RANK ne nahi

ROW_NUMBER   har row alag number
RANK         barabar = same, phir number CHHOOT-ta   (1,2,2,4)
DENSE_RANK   barabar = same, kuch nahi chhoot-ta    (1,2,2,3)
```

## CLASSIC: har department me 2nd highest salary
```sql
SELECT * FROM (
    SELECT name, dept, salary,
           DENSE_RANK() OVER (PARTITION BY dept ORDER BY salary DESC) AS rnk
    FROM employees
) t
WHERE rnk = 2;
```
★ window function WHERE me seedha nahi likh sakte (WHERE pehle chalta hai) -> subquery / CTE me lapeto.

## Baaki kaam ke (naam yaad rakho)
```
LAG(col)  OVER (ORDER BY day)   pichhli row ki value   -> "kal se kitna badha?"
LEAD(col) OVER (ORDER BY day)   agli row ki value
```
