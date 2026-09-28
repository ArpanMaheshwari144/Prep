# PR DRILLS — code + answer key (redo ke liye)

> Tareeka = [PR_REVIEW.md](PR_REVIEW.md) section 1 (shikaar list). Answer key code dikhane se PEHLE likhi.
> Redo: code padho, key band rakho, 10 min, phir key se milao.

---

## DRILL 1 — StatementJob (nightly statement email)

```java
 1  public class StatementJob {
 2
 3      static List<String> failed = new ArrayList<>();
 4      private boolean running = false;
 5
 6      private String dbUrl  = "jdbc:mysql://prod-db:3306/bank";
 7      private String dbUser = "admin";
 8      private String dbPass = "Admin@123";
 9
10      public void run(String date) {
11          running = true;
12          try {
13              Connection c = DriverManager.getConnection(dbUrl, dbUser, dbPass);
14              Statement s = c.createStatement();
15              ResultSet r = s.executeQuery(
16                  "SELECT id, email, status FROM accounts WHERE stmt_date = '" + date + "'");
17              while (r.next()) {
18                  String e  = r.getString("email");
19                  String st = r.getString("status");
20                  if (st == "ACTIVE") {
21                      System.out.println("sending statement to " + e);
22                      boolean x = send(e, build(r.getLong("id")));
23                      if (!x) failed.add(e);
24                  }
25              }
26          } catch (Exception ex) {
27              System.out.println("error");
28          }
29          running = false;
30      }
31
32      public boolean isRunning() { return running; }
33
34      private String build(long id) throws SQLException {
35          Connection c = DriverManager.getConnection(dbUrl, dbUser, dbPass);
36          ResultSet r = c.createStatement().executeQuery(
37              "SELECT amount, txn_date FROM txns WHERE account_id = " + id);
38          String out = "";
39          while (r.next()) {
40              out = out + r.getDouble("amount") + " " + r.getDate("txn_date") + "\n";
41          }
42          return out;
43      }
44
45      private boolean send(String to, String body) {
46          try {
47              mailClient.send(to, body);
48              return true;
49          } catch (Exception e) {
50              return false;
51          }
52      }
53  }
```

<details><summary>ANSWER KEY (17)</summary>

```
BHAARI
 1  L6-8    hardcoded DB url / user / password -> env / Vault
 2  L16,37  SQL injection (date, id string me jode) -> PreparedStatement + ?
 3  L13,35  Connection / Statement / ResultSet close nahi; build() har account pe NAYA connection
            -> try-with-resources / pool / JdbcTemplate
 4  L20     String == "ACTIVE" -> kabhi true nahi, job ek bhi mail nahi bhejegi -> "ACTIVE".equals(st)
 5  L26-27  catch (Exception) + println("error") -> kya toota pata nahi -> log.error(.., ex)
 6  L49-50  send() exception nigla, log nahi
 7  L3      static mutable List -> sab share, kabhi saaf nahi, ArrayList thread-safe nahi
 8  L4      running doosra thread padhta -> volatile nahi; run() dobara chal sakta -> AtomicBoolean.compareAndSet
 9  L22/37  loop me har account pe query = N+1 -> ek JOIN
10  L40     double me amount -> BigDecimal
11  L21     email (PII) log me
STYLE
12  naam: c, s, r, e, st, x, build
13  L40     loop me String + -> StringBuilder
14  L20     "ACTIVE" magic string -> enum / constant
15  L29     running = false -> finally
16  design: DB + format + mail ek class me (SRP)
17  failed list ka koi use nahi (retry / report nahi)

FAISLA: production me NAHI — creds + SQL injection + connection leak + String == (koi mail nahi jaayegi)
```
</details>

Pehli baar (28-Sep): 17 me se 7 + ek sahi point key se bahar (L21 println -> logger).
Chhoote: SQL injection · connection leak · static · N+1 · PII · StringBuilder · magic string ·
finally / AtomicBoolean · SRP · faisla line.
