package com.example.practice.repository;

/*
 * ============================================================
 * YE KYA HAI: Repository — DB se baat karne wala layer. CRUD + custom queries. JPA sab free mein deta.
 * KYUN CHAHIYE: DB operations ka code baar baar likhna padta. JpaRepository se sab FREE milta.
 * KAB USE: Jab bhi DB se data chahiye — Service layer isko call karegi.
 *
 * REAL WORLD ANALOGY:
 *   Repository = LIBRARIAN (library ka).
 *   Tu jaake bolo: "Arpan ki book chahiye" (findByName("Arpan")).
 *   Librarian jaake shelf (DB) se dhundh ke laaye.
 *   Tu shelf mein khud nahi gaya — librarian (repository) ne handle kiya.
 *   Bina librarian: tu khud 10,000 books mein dhundh — SLOW + error prone.
 *
 * JDBC MEIN KAISE HOTA (bina Spring Data JPA):
 *   // findAll() ke liye MANUALLY likhna padta:
 *   Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/student_db", "root", "root");
 *   String sql = "SELECT * FROM students";
 *   PreparedStatement ps = conn.prepareStatement(sql);
 *   ResultSet rs = ps.executeQuery();
 *   List<Student> list = new ArrayList<>();
 *   while (rs.next()) {
 *       Student s = new Student();
 *       s.setId(rs.getLong("id"));         // column naam MANUALLY likhna
 *       s.setName(rs.getString("name"));   // ek ek field MANUALLY set
 *       s.setEmail(rs.getString("email"));
 *       s.setAge(rs.getInt("age"));
 *       list.add(s);
 *   }
 *   rs.close(); ps.close(); conn.close();  // close bhool gaye toh CONNECTION LEAK
 *   // = 15 lines sirf ek SELECT * ke liye
 *
 * SPRING DATA JPA MEIN:
 *   studentRepository.findAll();    // 1 LINE. Bas. SQL + mapping + connection sab auto.
 *
 * HIBERNATE INTERNALLY KYA KARTA:
 *   1. HikariCP connection pool se ek connection le
 *   2. SQL generate: "SELECT s1_0.id, s1_0.name, s1_0.email, s1_0.age FROM students s1_0"
 *   3. JDBC PreparedStatement execute
 *   4. ResultSet → Student objects AUTOMATIC map (@Entity dekhke)
 *   5. Connection pool ko connection wapas de
 *   Tu 1 line likha — andar 5 steps hua.
 * ============================================================
 */

import com.example.practice.entity.Student;
import org.springframework.data.jpa.repository.JpaRepository;  // Spring Data JPA ka magic interface
import org.springframework.data.jpa.repository.Query;           // Custom query annotation
import org.springframework.data.repository.query.Param;         // Query parameter bind
import org.springframework.stereotype.Repository;              // Spring bean — DB access layer
import java.util.List;
import java.util.Optional;

/*
 * ============================================================
 * @Repository
 *
 * YE KYA HAI: Spring ko bolo "ye DB ACCESS layer hai." Isko bean banao.
 *
 * KYUN LAGAYA:
 *   1. Spring isko bean banata (singleton object, ApplicationContext mein store)
 *   2. BONUS: DB exceptions → Spring DataAccessException mein convert
 *      (SQLException → DataAccessException — generic, DB independent)
 *   3. Component scan mein detect hota
 *   4. Code padh ke INTENT clear: "ye data layer hai, business logic nahi"
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Actually JpaRepository extend kiya hai toh Spring AUTO DETECT karta — @Repository optional
 *   - BUT good practice lagana — code readability + exception translation bonus
 *   - Agar JpaRepository extend NAHI kiya hota aur @Repository nahi lagaya →
 *     Spring bean nahi banayega → @Autowired fail → NullPointerException CRASH
 *
 * REAL WORLD ANALOGY:
 *   @Repository = LIBRARIAN ka ID CARD.
 *   ID card dikha → building (Spring) mein entry → apna kaam kar sakta.
 *   JpaRepository extend kiya = librarian ka beta hai, toh baap ke basis pe entry mil jaati (auto detect).
 *   BUT ID card rakhna accha hai — sab jaante ye LIBRARIAN hai, koi aur nahi.
 *
 * @Repository vs @Component:
 *   Functionally same. @Repository INTENT batata (ye data layer hai).
 *   Plus exception translation bonus milta.
 * ============================================================
 */
@Repository
public interface StudentRepository extends JpaRepository<Student, Long> {
    /*
     * ============================================================
     * extends JpaRepository<Student, Long>
     *
     * YE KYA HAI: Spring Data JPA ka magic interface. Extend karo → CRUD methods FREE.
     *
     * KYUN LAGAYA: Ek line extend karo → 10+ DB methods bina ek line SQL likhe mil jaate.
     *   Student = kaunsi Entity? Student table.
     *   Long    = Primary Key ka type? Long (id field).
     *
     * NAHI LAGAYA TO KYA HOGA (JpaRepository extend nahi kiya):
     *   - save(), findById(), findAll(), deleteById() — sab KHUD likhna padega
     *   - Har method mein JDBC code — SQL, PreparedStatement, ResultSet mapping
     *   - 100+ lines ka boilerplate SIRF basic CRUD ke liye
     *   - Connection management, transaction handling — sab MANUAL
     *
     * REAL WORLD ANALOGY:
     *   JpaRepository extend karna = FRANCHISE lena (like McDonald's).
     *   Franchise li → recipe, supply chain, brand SAB FREE mila (save, find, delete).
     *   Khud se restaurant banao → recipe khud banao, supply chain khud set karo — MONTHS lagenge.
     *
     * EXTEND KARTE HI FREE MILTA (implementation Spring auto deta):
     *   save(student)        — INSERT ya UPDATE (id hai toh update, nahi toh insert)
     *   findById(id)         — SELECT * FROM students WHERE id = ?
     *   findAll()            — SELECT * FROM students
     *   findAll(Pageable)    — SELECT * FROM students LIMIT ? OFFSET ? ORDER BY ?
     *   deleteById(id)       — DELETE FROM students WHERE id = ?
     *   count()              — SELECT COUNT(*) FROM students
     *   existsById(id)       — SELECT 1 FROM students WHERE id = ? (boolean return)
     *
     * EK LINE SQL NAHI LIKHI — Spring ne sab implement kar diya.
     * Tu INTERFACE likha — implementation Spring ne RUNTIME pe proxy class bana ke di.
     *
     * PROXY KYA HAI:
     *   Runtime pe Spring ek class generate karta jo ye interface implement karta.
     *   SimpleJpaRepository — ye Spring ki default implementation hai.
     *   Har method mein EntityManager use karke Hibernate se SQL execute.
     * ============================================================
     */

    // ==========================================
    // TARIKA 1: DERIVED QUERY (Method naam se auto query)
    // Spring method ka NAAM parse karta aur SQL BANATA. Magic!
    // ==========================================

    /*
     * ============================================================
     * findByEmail() — Derived Query
     *
     * YE KYA HAI: Spring method NAAM parse karta aur SQL auto generate karta.
     *   "findBy" → SELECT query banao
     *   "Email"  → WHERE email = ? condition lagao
     *
     * KYUN LAGAYA: SQL likhne ki zaroorat nahi. Method naam likh do, Spring samajh jaayega.
     *
     * NAHI LAGAYA TO (method nahi banayi):
     *   - @Query se manually SQL likhna padega
     *   - Ya JDBC mein 10+ lines code
     *
     * MYSQL GENERATE: SELECT * FROM students WHERE email = ?
     *
     * Optional<Student> kyun?
     *   Student return karte toh → nahi mila? → null → NullPointerException chance
     *   Optional return karte toh → nahi mila? → Optional.empty() → safe handling
     *   .orElseThrow() — nahi mila toh exception
     *   .orElse(defaultValue) — nahi mila toh default
     *   .isPresent() — mila ya nahi check
     *
     * REAL WORLD ANALOGY:
     *   Tu librarian se bolo: "findByEmail → email se dhundh" → librarian samajh gaya.
     *   Bina ye method: tu khud shelf (DB) mein jaake dhundh — SQL likh.
     * ============================================================
     */
    Optional<Student> findByEmail(String email);

    /*
     * ============================================================
     * findByName() — Derived Query
     *
     * "findBy" + "Name" → SELECT * FROM students WHERE name = ?
     * List<Student> — multiple results aa sakte (same name ke multiple students)
     *
     * AUR BHI DERIVED QUERY EXAMPLES:
     *   findByNameAndAge(name, age)         → WHERE name = ? AND age = ?
     *   findByNameOrEmail(name, email)      → WHERE name = ? OR email = ?
     *   findByNameContaining("rp")          → WHERE name LIKE '%rp%'
     *   findByNameStartingWith("Ar")        → WHERE name LIKE 'Ar%'
     *   findByAgeBetween(18, 30)            → WHERE age BETWEEN 18 AND 30
     *   findByAgeIn(List.of(20, 25, 30))    → WHERE age IN (20, 25, 30)
     *   findByNameOrderByAgeDesc(name)      → WHERE name = ? ORDER BY age DESC
     *   countByAge(25)                       → SELECT COUNT(*) WHERE age = 25
     *   existsByEmail(email)                 → boolean — hai ya nahi
     * ============================================================
     */
    List<Student> findByName(String name);

    /*
     * ============================================================
     * findByAgeGreaterThan() — Derived Query
     *
     * "GreaterThan" → WHERE age > ?
     *
     * COMPARISON KEYWORDS:
     *   GreaterThan      → >
     *   LessThan         → <
     *   GreaterThanEqual → >=
     *   LessThanEqual    → <=
     *   Not              → !=
     *   IsNull           → IS NULL
     *   IsNotNull        → IS NOT NULL
     * ============================================================
     */
    List<Student> findByAgeGreaterThan(int age);

    // ==========================================
    // TARIKA 2: NATIVE SQL (Raw SQL — seedha DB pe)
    // ==========================================

    /*
     * ============================================================
     * @Query(nativeQuery = true) — NATIVE QUERY
     *
     * YE KYA HAI: Raw SQL likho, DB pe directly execute. Tu SQL MASTER hai toh ye use karo.
     *
     * KYUN LAGAYA: Derived query se complex SQL nahi ban sakti. Raw SQL likhna padta.
     *   nativeQuery = true → ye RAW SQL hai, JPQL nahi.
     *   "students" = TABLE NAAM (class nahi!) — kyunki ye seedha MySQL pe chalegi.
     *   :age = named parameter — @Param("age") se bind hoga.
     *
     * NAHI LAGAYA TO (NATIVE QUERY nahi use ki):
     *   - Complex queries ke liye bahut lambi derived method naam likhni padegi
     *   - DB specific functions (MySQL: DATEDIFF, GROUP_CONCAT) use nahi kar paoge
     *   - Performance critical queries optimize nahi kar paoge
     *
     * REAL WORLD ANALOGY:
     *   Derived Query = Zomato pe order dena (limited menu).
     *   Native SQL = khud kitchen mein jaake bolo "aisa banao" (full control).
     *   Kitchen (DB) ka expert hai toh seedha bolo kya chahiye.
     *
     * PROBLEM: DB dependent. MySQL syntax likhi → PostgreSQL pe break kar sakti.
     *   JPQL prefer karo jab bhi ho sake.
     * ============================================================
     */
    @Query(value = "SELECT * FROM students WHERE age > :age", nativeQuery = true)
    List<Student> findStudentsOlderThan(@Param("age") int age);
    /*
     * @Param("age"):
     *   YE KYA HAI: Method parameter "age" ko query mein ":age" se BIND karta.
     *   NAHI LAGAYA TO: Spring ko pata nahi konsa parameter kahan jaaye → error.
     *   JDBC mein: ps.setInt(1, age) — position based (galti ka chance).
     *   YAHAN: @Param("age") — naam based (readable, safe).
     */

    // ==========================================
    // TARIKA 3: JPQL (Java Persistence Query Language)
    // ==========================================

    /*
     * ============================================================
     * @Query (JPQL) — Entity based query. DB INDEPENDENT.
     *
     * YE KYA HAI: JPA ki apni query language. Entity/class pe kaam karta, table pe nahi.
     *
     * KYUN LAGAYA: DB switch karo (MySQL → PostgreSQL) → query SAME chalegi.
     *   Hibernate JPQL → SQL translate karta based on dialect.
     *
     * "Student" = CLASS NAAM (table nahi!) — JPQL entity pe kaam karta.
     * "s.email" = FIELD NAAM (column nahi!) — Java field use karo.
     * "s" = alias (SQL mein bhi hota — SELECT s.* FROM students s)
     *
     * JPQL vs NATIVE SQL:
     *   JPQL:   SELECT s FROM Student s WHERE s.email LIKE ...  (CLASS/FIELD naam)
     *   NATIVE: SELECT * FROM students WHERE email LIKE ...     (TABLE/COLUMN naam)
     *
     * NAHI LAGAYA TO (JPQL nahi, native SQL use ki):
     *   - DB switch kiya → sab queries rewrite
     *   - DB independent code nahi rahega
     *
     * REAL WORLD ANALOGY:
     *   JPQL = HINDI mein baat karo (translator — Hibernate — local language mein convert karega).
     *   Native SQL = LOCAL LANGUAGE mein seedha bolo (MySQL specific).
     *   Hindi bola → Bihar jaao ya UP → sab samjhenge (DB independent).
     *   Bhojpuri bola → sirf Bihar mein chalega (DB specific).
     *
     * HIBERNATE INTERNALLY:
     *   JPQL parse → HQL → SQL generate based on MySQL dialect:
     *   "SELECT s1_0.id, s1_0.name, s1_0.email, s1_0.age FROM students s1_0
     *    WHERE s1_0.email LIKE CONCAT('%', ?)"
     * ============================================================
     */
    @Query("SELECT s FROM Student s WHERE s.email LIKE %:domain")
    List<Student> findByEmailDomain(@Param("domain") String domain);
}
