package com.example.practice.entity;

/*
 * ============================================================
 * YE KYA HAI: Entity class — DB table ka Java version. Har field = column. Har object = row.
 * KYUN CHAHIYE: Hibernate ko batana padta ki kaunsa class kaunse table se map hoga.
 * KAB USE: Jab bhi DB mein data save/read/update/delete karna ho.
 *
 * REAL WORLD ANALOGY:
 *   Entity = ek FORM (like admission form).
 *   Har form ka format same (class), har filled form = ek row (object).
 *   Form ke fields (name, email, age) = table ke columns.
 *   Hibernate ye form dekhke apne register (DB table) mein entry karta.
 *
 * HIBERNATE YE CLASS DEKHKE KYA KARTA (ddl-auto=update pe):
 *   CREATE TABLE students (
 *       id BIGINT AUTO_INCREMENT PRIMARY KEY,
 *       name VARCHAR(255) NOT NULL,
 *       email VARCHAR(255) NOT NULL UNIQUE,
 *       age INT NOT NULL
 *   );
 *   Tu SQL nahi likha — Hibernate ne annotations dekhke generate kiya.
 *
 * JDBC MEIN KARNA PADTA (bina Hibernate):
 *   String sql = "INSERT INTO students (name, email, age) VALUES (?, ?, ?)";
 *   PreparedStatement ps = connection.prepareStatement(sql);
 *   ps.setString(1, "Arpan");
 *   ps.setString(2, "arpan@gmail.com");
 *   ps.setInt(3, 25);
 *   ps.executeUpdate();
 *   — 6 lines sirf ek INSERT ke liye. Hibernate mein: repository.save(student) — 1 line.
 *
 * ORM (Object Relational Mapping):
 *   Java Object <-> DB Table mapping.
 *   Student class  → students table
 *   Student object → ek row
 *   name field     → name column
 *   Ye mapping AUTOMATIC hai — Hibernate handle karta.
 * ============================================================
 */

import jakarta.persistence.*;        // JPA annotations — DB mapping ke liye
import jakarta.validation.constraints.*;  // Validation annotations — input check
import lombok.*;                    // Lombok — boilerplate code auto generate

/*
 * ============================================================
 * @Entity (JPA annotation)
 *
 * YE KYA HAI: YE CLASS = DB TABLE hai. Hibernate isko table se map karega.
 *
 * KYUN LAGAYA: Bina iske Hibernate ko pata hi nahi ki ye class DB se related hai.
 *   Hibernate EntityManager is class ko track karta.
 *   save() karo → EntityManager dekhta @Entity hai → SQL INSERT generate → execute.
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Hibernate isko COMPLETELY IGNORE karega — save/find kuch nahi hoga
 *   - repository.save(student) → "Not a managed type: Student" ERROR
 *   - Table bhi nahi banega DB mein (ddl-auto=update pe bhi nahi)
 *   - JpaRepository<Student, Long> extend kiya but Student entity nahi → CRASH at startup
 *
 * REAL WORLD ANALOGY:
 *   @Entity = tera AADHAAR CARD enrollment.
 *   Aadhaar enroll kiya → government system mein exist karta (DB mein table hai).
 *   Enroll nahi kiya → government ke liye tu EXIST nahi karta (Hibernate ignore karega).
 *
 * JPA vs Hibernate:
 *   @Entity = JPA ka annotation (standard interface — like rules ka book)
 *   Hibernate = JPA ka implementation (actual kaam karne wala — rules follow karta)
 *   Tu JPA annotations use karo → Hibernate implement karta.
 *   Kal EclipseLink pe switch karo → code SAME, sirf config badlo.
 * ============================================================
 */
@Entity

/*
 * ============================================================
 * @Table(name = "students")
 *
 * YE KYA HAI: DB mein table ka naam explicitly set karo — "students".
 *
 * KYUN LAGAYA: Naming convention follow karne ke liye.
 *   Convention: plural, lowercase — "students", "courses", "users".
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Table naam = class naam "Student" (capital S, singular)
 *   - Kuch DB mein case-sensitive hota → "Student" aur "student" ALAG tables
 *   - MySQL pe usually koi dikkat nahi, but PostgreSQL pe case matter karta
 *   - Convention break hoga — team mein sab confused ki table naam kya hai
 *
 * REAL WORLD ANALOGY:
 *   @Table(name) = apne SHOP ka NAAM BOARD lagana.
 *   Nahi lagaya toh shop ka naam = owner ka naam (class naam) — confusing.
 *   "students" board lagaya → sab jaante kya hai.
 * ============================================================
 */
@Table(name = "students")

/*
 * ============================================================
 * @Data (Lombok annotation)
 *
 * YE KYA HAI: Compile time pe getter, setter, toString, equals, hashCode SAB auto generate.
 *
 * KYUN LAGAYA: 50+ lines ka boilerplate EK annotation se khatam.
 *   @Data = compile time pe ye code AUTO GENERATE:
 *     public Long getId() { return id; }
 *     public void setId(Long id) { this.id = id; }
 *     public String getName() { return name; }
 *     ... har field ka getter/setter
 *     public String toString() { return "Student(id=1, name=Arpan...)"; }
 *     public boolean equals(Object o) { ... }
 *     public int hashCode() { ... }
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Har field ka getter/setter MANUALLY likhna padega (4 fields = 8 methods minimum)
 *   - toString() nahi → debugging mein "Student@4a5e7" dikhega — kya hai pata nahi
 *   - equals/hashCode nahi → Collections (Set, Map) mein wrong behavior
 *   - Jackson (@RequestBody/@ResponseBody) getter/setter dhundhega → nahi milega → JSON parse FAIL
 *
 * REAL WORLD ANALOGY:
 *   @Data = SERVANT rakh liya ghar mein.
 *   Bina servant: khud chai bana, khud bartan dho, khud safai kar (manually getter/setter likh).
 *   Servant rakh liya → bol do kya chahiye, ho jaayega (Lombok kar dega).
 *
 * HOW IT WORKS: Annotation Processor. Compile time pe bytecode modify karta.
 *   IDE mein Lombok plugin chahiye — nahi toh IDE red lines dikhayega (but compile hoga).
 * ============================================================
 */
@Data
/*
 * @NoArgsConstructor (Lombok): empty constructor — new Student() bina argument ke.
 *   KYUN: JPA/Hibernate ko ZAROORI hai — reflection se object banata hai. Bina iske → InstantiationException.
 *   NAHI LAGAYA TO: Hibernate new Student() nahi kar payega → "No default constructor" ERROR.
 */
@NoArgsConstructor
/*
 * @AllArgsConstructor (Lombok): full constructor — new Student(id, name, email, age) sab argument ke saath.
 *   KYUN: Convenient hai — ek line mein poora object bana sakte.
 *   NAHI LAGAYA TO: Har field manually set karna padega — s.setName(...); s.setEmail(...); TEDIOUS.
 */
@AllArgsConstructor
public class Student {

    /*
     * ============================================================
     * @Id (JPA annotation)
     *
     * YE KYA HAI: Ye field PRIMARY KEY hai — har student ka UNIQUE identifier.
     *
     * KYUN LAGAYA: Hibernate ko batana padta ki kaunsa field primary key hai.
     *   Har entity mein EK @Id ZAROORI hai. Bina iske entity incomplete.
     *   Hibernate isko primary key maan ke sab operations karega (findById, deleteById, etc.)
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - "No identifier specified for entity: Student" ERROR at startup
     *   - App START hi nahi hogi — Hibernate crash karega
     *   - findById(), deleteById() — sab methods use nahi kar paoge
     *   - JpaRepository primary key ke basis pe kaam karta — key nahi toh kaam nahi
     *
     * REAL WORLD ANALOGY:
     *   @Id = tera ROLL NUMBER school mein.
     *   Roll number se teacher dhundhta — "Roll no. 5 kaun?" → turant mil gaya.
     *   Roll number nahi → teacher ko poori class scan karni padegi → SLOW + confusion.
     *   DB mein: PRIMARY KEY (id) — auto index banta, fastest lookup.
     * ============================================================
     */
    @Id

    /*
     * ============================================================
     * @GeneratedValue(strategy = GenerationType.IDENTITY)
     *
     * YE KYA HAI: AUTO INCREMENT — DB khud id assign karega (1, 2, 3, 4...)
     *   Tu id SET mat kar — save() pe DB next number dega automatically.
     *
     * KYUN LAGAYA: Manually id assign karna = galti ka chance (duplicate, skip, etc.)
     *   DB ko de do — wo perfect sequence maintain karega.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Har student banate waqt ID KHUD set karna padega: student.setId(101L)
     *   - ID kya dena hai ye track karna TERA kaam — conflict/duplicate chance HIGH
     *   - Bhool gaye id set karna → id = null → DB error (primary key null nahi ho sakta)
     *
     * REAL WORLD ANALOGY:
     *   @GeneratedValue = BANK ka TOKEN MACHINE.
     *   Tu jaao → machine automatically next number de — 101, 102, 103...
     *   Bina machine: khud decide karo number, duplicate ho gaya → chaos.
     *
     * STRATEGIES EXPLAINED:
     *   IDENTITY   — DB auto increment (MySQL default, RECOMMENDED for MySQL)
     *   SEQUENCE   — DB sequence object use (PostgreSQL mein common, RECOMMENDED for PostgreSQL)
     *   TABLE      — alag table mein counter rakh (SLOW, avoid karo)
     *   AUTO       — Hibernate decide kare DB pe depend karke (unpredictable — avoid)
     *
     * MYSQL MEIN: id BIGINT AUTO_INCREMENT — har INSERT pe id++ automatic.
     * ============================================================
     */
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    /*
     * ============================================================
     * @NotBlank(message = "Name is required")
     *
     * YE KYA HAI: Validation annotation — name field blank nahi hona chahiye.
     *   null → REJECT. "" (empty) → REJECT. "   " (spaces) → REJECT.
     *   Sirf String pe kaam karta (int/Long pe nahi — unke liye @NotNull use karo).
     *
     * KYUN LAGAYA: Bina naam ke student ka kya matlab? Garbage data DB mein nahi jaana chahiye.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Client blank name bhejega → DB mein "" save ho jaayega → garbage data
     *   - Baad mein reports, search — sab mein blank entries aayengi
     *   - LEKIN SIRF TAB KAAM KAREGA jab Controller mein @Valid lagao!
     *     @Valid nahi lagaya → @NotBlank DEAD hai — kuch nahi karega.
     *
     * REAL WORLD ANALOGY:
     *   @NotBlank = school admission form pe "NAME" field ke saath * (required) mark.
     *   Blank chhod ke form submit kiya → clerk reject karega — "naam toh bhar!"
     *   * nahi lagaya → clerk accept kar lega blank form → records mein gadbad.
     *
     * @NotNull vs @NotBlank vs @NotEmpty:
     *   @NotNull  — null reject. "" aur "   " PASS. (least strict)
     *   @NotEmpty — null aur "" reject. "   " PASS. (medium strict)
     *   @NotBlank — null, "", "   " SAB reject. (STRICTEST — String ke liye best)
     * ============================================================
     */
    @NotBlank(message = "Name is required")
    private String name;

    /*
     * ============================================================
     * @Email(message = "Invalid email")
     *
     * YE KYA HAI: Regex se email format check karta.
     *   "abc" → REJECT (@ nahi hai).
     *   "abc@" → REJECT (domain nahi).
     *   "abc@xyz.com" → PASS.
     *   Internally RFC standard regex pattern match karta.
     *
     * KYUN LAGAYA: Bina iske koi bhi random string email ban jaayega.
     *   "hello" bhi email ban jaayega — fir email bhejne pe fail.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - "abc", "123", "random text" — sab valid email maan lega
     *   - DB mein garbage emails store honge
     *   - Notification/email bhejne pe sab FAIL — invalid addresses
     *   - Data quality = ZERO
     *
     * REAL WORLD ANALOGY:
     *   @Email = POST OFFICE ka address format check.
     *   "Arpan, XYZ Colony, Delhi, 110001" → valid address → letter jayega.
     *   "just Arpan" → postman bole "ye toh address hi nahi hai!" → reject.
     * ============================================================
     */
    @Email(message = "Invalid email")

    /*
     * @NotBlank on email — email bhi blank nahi hona chahiye.
     *   NAHI LAGAYA TO: null ya empty email save ho jaayega.
     */
    @NotBlank(message = "Email is required")

    /*
     * ============================================================
     * @Column(unique = true)
     *
     * YE KYA HAI: DB LEVEL pe UNIQUE constraint. Same email dobara INSERT → SQLException.
     *
     * KYUN LAGAYA: Ek email = ek student. Duplicate emails mean data corruption.
     *   Ye DB LEVEL protection hai — Java validation bypass bhi ho toh DB reject karega.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Same email se 10 students register ho jayenge → data corruption
     *   - "Find by email" pe multiple results → code confusion
     *   - Login system mein: same email, alag passwords → kaun asli?
     *   - Business logic completely break ho jaayegi
     *
     * REAL WORLD ANALOGY:
     *   @Column(unique) = AADHAAR NUMBER system.
     *   Har Indian ka unique Aadhaar. Duplicate Aadhaar? IMPOSSIBLE — system reject karega.
     *   Bina unique: 5 logon ka same Aadhaar → kaun asli? Identity crisis!
     *
     * HIBERNATE YE BANATA:
     *   ALTER TABLE students ADD CONSTRAINT UK_email UNIQUE (email);
     *   Ya CREATE TABLE mein: email VARCHAR(255) UNIQUE
     *   Automatically UNIQUE INDEX bhi banta → email se search FAST.
     *
     * MYSQL MEIN:
     *   INSERT INTO students (name, email, age) VALUES ('A', 'same@email.com', 20);  -- OK
     *   INSERT INTO students (name, email, age) VALUES ('B', 'same@email.com', 22);  -- ERROR! Duplicate entry
     * ============================================================
     */
    @Column(unique = true)
    private String email;

    /*
     * ============================================================
     * @Min(value = 18, message = "Age must be at least 18")
     *
     * YE KYA HAI: Numeric value 18 se kam → REJECT.
     *   age = 17 → "Age must be at least 18" error.
     *   age = 18 → PASS.
     *
     * KYUN LAGAYA: Minor students ko register nahi karna — business rule.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - age = 0, age = -5, age = 2 — sab valid maan lega
     *   - Illegal age values DB mein store honge
     *   - Reports mein nonsense data aayega
     *
     * REAL WORLD ANALOGY:
     *   @Min(18) = DRIVING LICENSE ka age check.
     *   17 saal ka aaya → "Sorry, 18+ chahiye" → reject.
     *   Check nahi kiya → 10 saal ka bachha bhi license le le → DANGEROUS.
     *
     * NOTE: Ye JAVA level validation hai — DB mein CHECK constraint alag se lagana padta:
     *   ALTER TABLE students ADD CHECK (age >= 18);
     *   Hibernate ddl-auto=update pe CHECK constraint auto nahi lagata.
     * ============================================================
     */
    @Min(value = 18, message = "Age must be at least 18")
    private int age;
}
