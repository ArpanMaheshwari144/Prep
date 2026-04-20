package com.example.practice.service;

/*
 * ============================================================
 * YE KYA HAI: Service — business logic yahan. Controller se request aaye, Repository se data le, process kar.
 * KYUN CHAHIYE: Controller mein logic likhoge toh messy, non-testable, non-reusable hoga.
 * KAB USE: Controller har request pe Service ko call karega.
 *
 * REAL WORLD ANALOGY:
 *   Controller = WAITER (order leta, serve karta — logic nahi jaanta)
 *   Service    = CHEF (khaana banata — actual kaam yahan hota)
 *   Repository = STORE ROOM (ingredients laata — data access)
 *   Waiter khud khaana nahi banata. Chef khud serve nahi karta. Har kisi ka apna kaam.
 *
 * LAYERED ARCHITECTURE:
 *   Controller → Service → Repository → DB
 *   Har layer ka apna kaam. Ek layer doosre ka kaam nahi karta.
 *
 *   Controller: HTTP handle — request liya, response bheja. Logic NAHI.
 *   Service:    LOGIC — data process, validate, transform. HTTP nahi jaanta.
 *   Repository: DB — data laao, save karo. Logic nahi jaanta.
 *
 *   FAYDA:
 *   - Kal DB badli (MySQL → PostgreSQL) → sirf Repository badlo. Service/Controller same.
 *   - Kal API badli (REST → GraphQL) → sirf Controller badlo. Service/Repository same.
 *   - Testing: Service independently test ho sakta — Controller ya DB nahi chahiye.
 *
 * @SERVICE vs @COMPONENT:
 *   Functionally SAME — dono Spring bean banate.
 *   @Service = @Component + INTENT (ye business logic layer hai).
 *   Code padhne wale ko samajh aata: "ye service hai, logic yahan hoga."
 *   Interview mein: "Specialized stereotype annotation — semantic meaning deta."
 *
 * DEPENDENCY INJECTION (DI):
 *   @Autowired private StudentRepository studentRepository;
 *   Tu "new StudentRepository()" NAHI karta — Spring object de deta.
 *   Spring container mein StudentRepository ka bean dhundhta → inject karta.
 *   Ye = IoC (Inversion of Control) — Spring control karta, tu nahi.
 *
 *   BETTER TARIKA (Constructor Injection):
 *     private final StudentRepository repo;
 *     public StudentService(StudentRepository repo) { this.repo = repo; }
 *     // @Autowired ki zaroorat nahi — Spring auto inject karega
 *     // Fayda: field immutable (final), testing mein mock easy, NullPointer early catch
 * ============================================================
 */

import com.example.practice.dto.StudentDTO;
import com.example.practice.entity.Student;
import com.example.practice.repository.StudentRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cache.annotation.*;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;
import java.util.*;
import java.util.stream.Collectors;

/*
 * ============================================================
 * @Service
 *
 * YE KYA HAI: Spring ko bolo "ye BUSINESS LOGIC layer hai." Isko bean banao.
 *
 * KYUN LAGAYA:
 *   1. Spring isko BEAN banata — singleton object create, ApplicationContext mein store.
 *   2. @Autowired kahi bhi karo → Spring ye object de dega.
 *   3. SINGLETON — ek hi object poore app mein. 100 requests = same StudentService object.
 *      Naya object har request pe nahi banta — memory efficient, fast.
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Spring isko scan NAHI karega → bean NAHI banega
 *   - Controller mein @Autowired StudentService → StudentService = NULL
 *   - Koi bhi method call pe → NullPointerException CRASH!
 *   - App chalegi, but student create/read/update/delete → sab 500 Internal Server Error
 *
 * REAL WORLD ANALOGY:
 *   @Service = CHEF ka APPOINTMENT LETTER.
 *   Letter mila → restaurant (Spring) mein officially HIRED → kaam kar sakta.
 *   Letter nahi → restaurant ko pata hi nahi ki chef hai → waiter (controller) bole
 *   "chef chahiye" → manager (Spring) bole "kaun chef? Mere paas koi nahi" → NullPointerException.
 * ============================================================
 */
@Service
public class StudentService {

    /*
     * ============================================================
     * @Autowired — Dependency Injection
     *
     * YE KYA HAI: Spring container se StudentRepository ka bean dhundh ke yahan INJECT karo.
     *   Tu "new StudentRepository()" KABHI nahi likha — Spring ne diya.
     *
     * KYUN LAGAYA: Manual object creation = tight coupling. Spring injection = loose coupling.
     *   Testing mein mock inject kar sakte. DB change karo → code same.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - studentRepository = NULL rahega (Spring inject nahi karega)
     *   - studentRepository.save(student) → NullPointerException CRASH!
     *   - "Why is my repository null?" — ye #1 Spring beginner mistake hai
     *
     * REAL WORLD ANALOGY:
     *   @Autowired = ZOMATO DELIVERY.
     *   Tu ghar baithe order diya (annotation lagaya) → delivery boy (Spring) laake de gaya (inject).
     *   @Autowired nahi lagaya → order hi nahi diya → khaana aayega? NAHI → bhookha (null).
     *
     * ANDAR KYA HOTA:
     *   1. Spring dekhta: StudentService ko StudentRepository chahiye
     *   2. ApplicationContext mein StudentRepository bean dhundhta
     *   3. Reflection se is field mein set kar deta
     *   AGAR BEAN NAHI MILA: "No qualifying bean of type StudentRepository" error.
     * ============================================================
     */
    @Autowired
    private StudentRepository studentRepository;

    // ==========================================
    // CREATE — naya student save
    // ==========================================

    /*
     * ============================================================
     * @CacheEvict(value = "students", allEntries = true)
     *
     * YE KYA HAI: Cache se entries HATAO. Purana (stale) cache saaf karo.
     *   value = "students" → "students" naam ke cache mein se
     *   allEntries = true → SAARI entries hatao (poora cache saaf)
     *
     * KYUN LAGAYA: Naya student add kiya → purani list cache mein hai → OUTDATED ho gayi.
     *   getAllStudents() cache mein 10 students the → naya add kiya → cache mein abhi bhi 10.
     *   allEntries = true → poora "students" cache saaf → next getAllStudents() fresh DB se layega.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Naya student add kiya → lekin getAllStudents() purana cache return karta
     *   - Client ko lagta "student save nahi hua" — actually hua, cache purana hai
     *   - STALE DATA — sabse common caching bug
     *   - Users confused: "maine add kiya but list mein nahi dikh raha!"
     *
     * REAL WORLD ANALOGY:
     *   Cache = MENU CARD ka photocopy (print kiya hua).
     *   Naya dish add ki restaurant mein → photocopy pe toh purani dishes hain.
     *   @CacheEvict = purani photocopy FAAD DO → naya print karo (fresh DB se).
     *   Nahi faada → customer purana menu dekhega → naya dish order nahi kar payega.
     * ============================================================
     */
    @CacheEvict(value = "students", allEntries = true)
    public StudentDTO addStudent(StudentDTO dto) {
        Student student = dto.toEntity();   // DTO → Entity convert (client data → DB format)
        Student saved = studentRepository.save(student);
        /*
         * save() INTERNALLY (Hibernate):
         *   1. Check: id hai? → NAHI → INSERT
         *      INSERT INTO students (name, email, age) VALUES ('Arpan', 'arpan@gmail.com', 25)
         *   2. Check: id hai? → HAI → UPDATE
         *      UPDATE students SET name='Arpan', email='...', age=25 WHERE id=1
         *
         *   Ek method — INSERT ya UPDATE dono handle karta.
         *   Return: saved entity WITH generated id (DB ne auto increment kiya).
         *
         *   JDBC MEIN:
         *     String sql = "INSERT INTO students (name, email, age) VALUES (?, ?, ?)";
         *     PreparedStatement ps = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
         *     ps.setString(1, dto.getName());
         *     ps.setString(2, dto.getEmail());
         *     ps.setInt(3, dto.getAge());
         *     ps.executeUpdate();
         *     ResultSet keys = ps.getGeneratedKeys();  // auto-generated id nikalo
         *     = 7 lines. Yahan 1 line.
         */
        return StudentDTO.fromEntity(saved);  // Entity → DTO convert (DB format → client format)
    }

    // ==========================================
    // READ ALL — with Pagination + Sorting
    // ==========================================
    public Page<StudentDTO> getAllStudents(Pageable pageable) {
        /*
         * Pageable — Spring ka pagination object.
         *   Controller se aata: ?page=0&size=5&sort=name,asc
         *   Spring auto parse karke Pageable banata.
         *
         * HIBERNATE INTERNALLY:
         *   SELECT * FROM students ORDER BY name ASC LIMIT 5 OFFSET 0
         *   + SELECT COUNT(*) FROM students  (total count ke liye)
         *
         * Page<T> response:
         *   { "content": [...5 students...], "totalPages": 10,
         *     "totalElements": 50, "number": 0 (current page) }
         *
         * BINA PAGINATION: findAll() → 10 lakh records ek saath → memory full → app crash.
         * PAGINATION SE: 5 records per page → fast, memory safe.
         *
         * ANALOGY: Google search results 10 per page dikhata, 10 crore ek saath nahi.
         */
        return studentRepository.findAll(pageable)
                .map(StudentDTO::fromEntity);
        /*
         * .map(StudentDTO::fromEntity) — Method Reference:
         *   Page<Student> → Page<StudentDTO> convert.
         *   Har Student object pe StudentDTO.fromEntity() call.
         *   Same as: .map(student -> StudentDTO.fromEntity(student))
         */
    }

    // ==========================================
    // READ BY ID
    // ==========================================

    /*
     * ============================================================
     * @Cacheable(value = "student", key = "#id")
     *
     * YE KYA HAI: Cache se de, DB SKIP. Pehli baar DB se le → cache mein rakh. Doosri baar cache se de.
     *   value = "student" → cache ka naam (like HashMap ka naam)
     *   key = "#id"       → cache key (like HashMap ka key)
     *
     * KYUN LAGAYA: Same student baar baar DB se laana = WASTE. Cache se do = FAST.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Har request pe DB call — 1000 users same student dekhe = 1000 DB calls
     *   - DB pe unnecessary LOAD — slow response time
     *   - Redis lagate hi expected performance improvement nahi milega
     *   - Paise waste — Redis server chala rahe but use nahi ho raha
     *
     * REAL WORLD ANALOGY:
     *   @Cacheable = SPEED DIAL phone mein.
     *   Pehli baar friend ka number SEARCH kiya (DB call) → speed dial mein SAVE kiya (cache).
     *   Doosri baar → speed dial se seedha call (cache hit) → search SKIP.
     *   1000 baar call karo → 1 baar search, 999 baar speed dial. FAST!
     *
     * FLOW DETAIL:
     *   getStudentById(1) — PEHLI BAAR:
     *     Spring proxy: cache mein "student::1" dhundha → NAHI MILA (cache miss)
     *     → method chala → DB se laya → cache mein "student::1" = result rakh diya → return
     *
     *   getStudentById(1) — DOOSRI BAAR:
     *     Spring proxy: cache mein "student::1" dhundha → MIL GAYA! (cache hit)
     *     → method CHALA HI NAHI → seedha cache se return → DB call ZERO
     *
     * REDIS MEIN STORE HOTA:
     *   KEY:   "student::1"
     *   VALUE: serialized StudentDTO JSON
     *   TTL:   config mein set kiya (60 min)
     *
     * AOP PROXY — KAISE KAAM KARTA:
     *   Spring tera StudentService wrap karta ek proxy mein.
     *   Jab getStudentById(1) call ho:
     *     Proxy intercept karta → cache check → miss? → original method call → cache store
     *   Tu ye sab nahi likhta — annotation lagao, Spring handle karta.
     *
     * GOTCHA (COMMON MISTAKE):
     *   Same class ke andar method call pe cache NAHI chalega!
     *   this.getStudentById(1) → proxy BYPASS → cache miss.
     *   Bahar se call hona chahiye (Controller se) → proxy through jaayega.
     * ============================================================
     */
    @Cacheable(value = "student", key = "#id")
    public StudentDTO getStudentById(Long id) {
        Student student = studentRepository.findById(id)
                .orElseThrow(() -> new RuntimeException("Student not found with id: " + id));
        /*
         * findById(id) → Optional<Student> return.
         *
         * Optional KYA HAI:
         *   Null safe wrapper. "Value hai ya nahi" explicitly batata.
         *   BINA Optional: Student s = repo.findById(id); → null aa sakta → NullPointerException
         *   WITH Optional: .orElseThrow() → nahi mila toh exception THROW (controlled)
         *
         * HIBERNATE INTERNALLY:
         *   SELECT * FROM students WHERE id = ?
         *   Result mila → Optional.of(student)
         *   Nahi mila → Optional.empty()
         *
         * orElseThrow():
         *   Student mila → return student.
         *   Nahi mila → RuntimeException throw → GlobalExceptionHandler pakdega → 404 JSON response.
         *
         * AUR OPTIONS:
         *   .orElse(defaultStudent)        — nahi mila toh default do
         *   .orElseGet(() -> createNew())  — nahi mila toh lazy create
         *   .isPresent()                   — boolean check
         *   .ifPresent(s -> doSomething()) — mila toh kuch karo
         */
        return StudentDTO.fromEntity(student);
    }

    // ==========================================
    // UPDATE
    // ==========================================

    /*
     * ============================================================
     * @CachePut(value = "student", key = "#id")
     *
     * YE KYA HAI: HAMESHA method chalao + cache UPDATE karo. Cache + DB dono sync mein.
     *
     * KYUN LAGAYA: Update ke baad cache mein PURANA data nahi rehna chahiye.
     *   DB mein naya data hai but cache mein purana → client ko GALAT data milega.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Student update kiya DB mein → cache mein PURANA data raha
     *   - GET kiya → @Cacheable cache se dega → PURANA (wrong) data return!
     *   - Client ko lagta "update nahi hua" — actually DB mein hua, cache stale hai
     *   - STALE CACHE = sabse dangerous caching bug
     *
     * REAL WORLD ANALOGY:
     *   @CachePut = NOTICE BOARD update karna.
     *   School mein exam schedule badla (DB update) →
     *   @CachePut = notice board PE BHI naya schedule lagao (cache update).
     *   Nahi lagaya → students purana schedule dekhenge → galat date pe aayenge → DISASTER.
     *
     * @Cacheable vs @CachePut:
     *   @Cacheable: cache mein hai? → HAI → DB SKIP, cache se de. (READ optimized)
     *   @CachePut:  cache mein hai? → FARAK NAHI PADTA → method HAMESHA chala → naya result cache mein. (WRITE optimized)
     * ============================================================
     */
    @CachePut(value = "student", key = "#id")
    public StudentDTO updateStudent(Long id, StudentDTO dto) {
        Student existing = studentRepository.findById(id)
                .orElseThrow(() -> new RuntimeException("Student not found with id: " + id));
        existing.setName(dto.getName());
        existing.setEmail(dto.getEmail());
        existing.setAge(dto.getAge());
        Student saved = studentRepository.save(existing);
        /*
         * save(existing) — id HAI toh UPDATE mode:
         *   HIBERNATE: UPDATE students SET name=?, email=?, age=? WHERE id=?
         *
         *   Hibernate DIRTY CHECKING bhi karta:
         *   Entity load ki → fields change kiye → transaction commit pe Hibernate dekhta
         *   kya change hua → sirf changed fields ka UPDATE generate karta.
         *   Ye = "dirty checking" — Hibernate automatically track karta kya badla.
         */
        return StudentDTO.fromEntity(saved);
    }

    // ==========================================
    // DELETE
    // ==========================================

    /*
     * ============================================================
     * @CacheEvict(value = "student", key = "#id")
     *
     * YE KYA HAI: Cache se EK entry hatao. Deleted student ka cache saaf karo.
     *   value = "student", key = "#id" → "student::5" entry REMOVE.
     *
     * KYUN LAGAYA: Student delete kiya DB se → cache mein kyun rakhe? Ghost data dikhega.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Student DB se deleted → lekin cache mein abhi bhi hai
     *   - GET /students/5 karo → cache se milega → "ye lo student!" → lekin DB mein hai NAHI!
     *   - GHOST DATA — deleted student ka bhoota cache mein ghoom raha
     *   - Client confused: "delete kiya tha, fir kaise aa raha?"
     *
     * REAL WORLD ANALOGY:
     *   Student school chhodh gaya → lekin uska naam ATTENDANCE REGISTER se nahi kataa.
     *   Teacher roz bole "Arpan?" → koi jawaab nahi → confusing.
     *   @CacheEvict = attendance register se naam KAAT DO (cache clean).
     * ============================================================
     */
    @CacheEvict(value = "student", key = "#id")
    public void deleteStudent(Long id) {
        Student student = studentRepository.findById(id)
                .orElseThrow(() -> new RuntimeException("Student not found with id: " + id));
        studentRepository.delete(student);
        /*
         * delete(student):
         *   HIBERNATE: DELETE FROM students WHERE id = ?
         *
         *   JDBC MEIN:
         *     String sql = "DELETE FROM students WHERE id = ?";
         *     PreparedStatement ps = conn.prepareStatement(sql);
         *     ps.setLong(1, id);
         *     ps.executeUpdate();
         *     conn.close();
         *     = 5 lines. Yahan 1 line.
         */
    }

    // ==========================================
    // SEARCH by name
    // ==========================================
    public List<StudentDTO> searchByName(String name) {
        return studentRepository.findByName(name).stream()
                .map(StudentDTO::fromEntity)
                .collect(Collectors.toList());
        /*
         * Java Streams:
         *   .stream()                    — List ko stream mein convert
         *   .map(StudentDTO::fromEntity) — har Student → StudentDTO convert
         *   .collect(Collectors.toList()) — stream wapas List mein
         *
         * HIBERNATE INTERNALLY:
         *   findByName("Arpan") → SELECT * FROM students WHERE name = 'Arpan'
         *   Method naam se query auto generate — tu SQL nahi likha.
         */
    }

    // ==========================================
    // SEARCH by age range
    // ==========================================
    public List<StudentDTO> searchByAgeRange(int minAge, int maxAge) {
        return studentRepository.findByAgeGreaterThan(minAge).stream()
                .filter(s -> s.getAge() <= maxAge)
                .map(StudentDTO::fromEntity)
                .collect(Collectors.toList());
        /*
         * HIBERNATE: SELECT * FROM students WHERE age > ?
         * Phir Java mein filter: age <= maxAge
         *
         * BETTER APPROACH: Repository mein findByAgeBetween(min, max) bana do:
         *   List<Student> findByAgeBetween(int min, int max);
         *   → SELECT * FROM students WHERE age BETWEEN ? AND ?
         *   DB level filtering = FAST (kam data transfer)
         *   Java level filtering = SLOW (sab data laao, phir filter)
         */
    }
}
