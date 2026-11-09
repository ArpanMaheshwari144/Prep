# SQL Joins (har join ka asli output, MySQL pe chala ke, 9-Nov)

## Do table

```
customers                  orders
+----+-------+            +-----+-------------+---------+
| id | name  |            | id  | customer_id | item    |
+----+-------+            +-----+-------------+---------+
|  1 | Arpan |            | 101 |           1 | Phone   |
|  2 | Ravi  |            | 102 |           1 | Cover   |
|  3 | Sita  |            | 103 |        NULL | Charger |
+----+-------+            +-----+-------------+---------+
```

Ravi aur Sita ne kuch nahi khareeda. Charger wala order kisi customer se juda nahi hai.

## INNER JOIN (sirf jinka dono taraf match ho)

```sql
SELECT c.name, o.id, o.item FROM customers c INNER JOIN orders o ON o.customer_id = c.id;
```
```
| Arpan | 101 | Phone |
| Arpan | 102 | Cover |
```

## LEFT JOIN (saare customer, order na mile to NULL)

```sql
... FROM customers c LEFT JOIN orders o ON o.customer_id = c.id;
```
```
| Arpan | 101  | Phone |
| Arpan | 102  | Cover |
| Ravi  | NULL | NULL  |
| Sita  | NULL | NULL  |
```

## RIGHT JOIN (saare order, customer na mile to NULL)

```sql
... FROM customers c RIGHT JOIN orders o ON o.customer_id = c.id;
```
```
| Arpan | 101 | Phone   |
| Arpan | 102 | Cover   |
| NULL  | 103 | Charger |
```

## FULL OUTER (dono taraf ke saare)

MySQL me ye seedha nahi hota (Postgres me hota hai), isliye LEFT `UNION` RIGHT likhte hain:

```
| Arpan | 101  | Phone   |
| Arpan | 102  | Cover   |
| Ravi  | NULL | NULL    |
| Sita  | NULL | NULL    |
| NULL  | 103  | Charger |
```

## CROSS JOIN (har customer x har order)

3 x 3 = **9 row**.

## SELF JOIN (employee ko usi table me uske manager se jodo)

```
employees:  1 Mehta (manager NULL) · 2 Arpan (manager 1) · 3 Ravi (manager 2)
```
```sql
SELECT e.name, m.name AS manager FROM employees e INNER JOIN employees m ON e.manager_id = m.id;
```
```
| Arpan | Mehta |
| Ravi  | Arpan |          <- Mehta gaayab, uska manager nahi
```
Wahi query LEFT JOIN se:
```
| Arpan | Mehta |
| Ravi  | Arpan |
| Mehta | NULL  |
```

## Trap: "kaunse customer ne kabhi order nahi kiya?"

```
WHERE id NOT IN (SELECT customer_id FROM orders)      -> khaali (Ravi, Sita dono gaayab)
LEFT JOIN orders o ... WHERE o.id IS NULL             -> Ravi, Sita
WHERE NOT EXISTS (SELECT 1 FROM orders o WHERE ...)   -> Ravi, Sita
```

Charger wale order ka `customer_id` NULL hai. `NOT IN` har customer ko us NULL se bhi milata hai, aur NULL se tulna ka jawab na sach hota
hai na jhooth, to koi bhi pass nahi hota. Isliye aisi query me `NOT EXISTS` ya `LEFT JOIN ... IS NULL` lo.

Doosra trap (DTO wali file me bhi): LEFT JOIN ke baad WHERE me right table ki shart lagayi to NULL wali rows kat jaati hain, aur LEFT
chup-chaap INNER ban jaata hai. Shart `ON` me likho.

MongoDB me join nahi hota, data andar hi rakhte hain; zaroorat pade to `$lookup`, jo LEFT JOIN jaisa hai.
