package com.example.practice.controller;

/*
 * ============================================================
 * YE KYA HAI: Controller — client ki request handle. URL mapping. Service ko call kare.
 * KYUN CHAHIYE: Client ko endpoints chahiye — GET, POST, PUT, DELETE. Controller route karta.
 * KAB USE: Jab client /api/students pe request bheje.
 *
 * REAL WORLD ANALOGY:
 *   Controller = RECEPTIONIST (hotel ka).
 *   Guest (client) aaya → receptionist (controller) pooche "kya chahiye?"
 *   Guest bole "room chahiye" (POST request) → receptionist housekeeping (service) ko bole → room ready.
 *   Guest bole "room check karna" (GET request) → receptionist records (service) se laaye → bataye.
 *   Receptionist KHUD kuch nahi karta — sirf route karta sahi department ko.
 *
 * DISPATCHER SERVLET FLOW (Request ka Journey):
 *   Client request → Tomcat → DispatcherServlet (front controller) →
 *     1. HandlerMapping: "GET /api/students/5 kiske liye?" → StudentController.getById()
 *     2. HandlerAdapter: method call karo, parameters resolve karo
 *     3. Method execute → ResponseEntity return
 *     4. HttpMessageConverter (Jackson): Java object → JSON convert
 *     5. Response bhejo client ko
 *
 *   BINA DispatcherServlet: Har URL ke liye alag Servlet likhna padta.
 *   50 APIs = 50 Servlet classes. DispatcherServlet se ek Servlet sab handle.
 *
 * REST API DESIGN:
 *   POST   /api/students       — CREATE (201 Created)
 *   GET    /api/students       — READ ALL (200 OK)
 *   GET    /api/students/{id}  — READ ONE (200 OK)
 *   PUT    /api/students/{id}  — UPDATE (200 OK)
 *   DELETE /api/students/{id}  — DELETE (200 OK)
 *
 *   HTTP Methods ka matlab:
 *     GET    = data LAAO (read only, server state change NAHI)
 *     POST   = naya BANAO (server pe naya resource create)
 *     PUT    = poora UPDATE (existing resource replace)
 *     PATCH  = partial UPDATE (sirf kuch fields)
 *     DELETE = HATAO (resource delete)
 * ============================================================
 */

import com.example.practice.dto.StudentDTO;
import com.example.practice.service.StudentService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.http.ResponseEntity;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import jakarta.validation.Valid;
import java.util.*;

/*
 * ============================================================
 * @RestController
 *
 * YE KYA HAI: 2-in-1 annotation = @Controller + @ResponseBody.
 *
 *   @Controller:
 *     Spring MVC controller mark. DispatcherServlet isko jaanta hai.
 *     HandlerMapping is class ke methods ko URL se map karta.
 *
 *   @ResponseBody:
 *     Return value ko HTTP response BODY mein daalo.
 *     Java object → JSON convert (Jackson library automatically).
 *     Bina @ResponseBody: Spring view name samjhega (HTML page dhundhega → error).
 *
 * KYUN LAGAYA: REST API bana rahe — hamesha JSON return karna hai, HTML nahi.
 *   @RestController = REST API ke liye = hamesha JSON return.
 *   @Controller = web pages ke liye = HTML return (Thymeleaf/JSP).
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Spring isko controller nahi samjhega → URL map NAHI hoga
 *   - Client request bhejega → 404 Not Found → "endpoint nahi mila"
 *   - Ya @Controller lagaya but @ResponseBody nahi → Spring HTML page dhundhega → error
 *
 * REAL WORLD ANALOGY:
 *   @RestController = restaurant ka MENU BOARD (bahar laga hua).
 *   Board hai → customer jaanta "ye restaurant hai, yahan khaana milta hai" (Spring jaanta ye controller hai).
 *   Board NAHI → customer guzar jaayega — "ye toh band dukaan hai" (404 Not Found).
 *
 * JACKSON LIBRARY (JSON Conversion):
 *   Java object <-> JSON conversion. Spring Boot mein auto-included.
 *   StudentDTO { name: "Arpan", age: 25 } → {"name":"Arpan","age":25}
 *   Request mein JSON aaya → @RequestBody → Jackson → Java object.
 *   Response mein Java object → Jackson → JSON → client ko.
 * ============================================================
 */
@RestController

/*
 * ============================================================
 * @RequestMapping("/api/students")
 *
 * YE KYA HAI: BASE URL set. Is class ke sab endpoints /api/students se start honge.
 *   @GetMapping("/{id}") → actual URL = GET /api/students/{id}
 *   @PostMapping         → actual URL = POST /api/students
 *
 * KYUN LAGAYA: Common prefix ek jagah likho — har method mein repeat mat karo. DRY principle.
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Har method mein FULL path likhna padega: @GetMapping("/api/students/{id}")
 *   - Path change karna ho → 6 jagah change karo → galti ka chance HIGH
 *   - Code MESSY — common prefix repeat ho raha
 *
 * REAL WORLD ANALOGY:
 *   @RequestMapping = building ka ADDRESS.
 *   "MG Road, Floor 3" = base address.
 *   Room 301 (@GetMapping) = MG Road, Floor 3, Room 301.
 *   Address nahi bataya → delivery boy (client) ko har baar full address dena padega.
 *
 * KYUN /api/ prefix?
 *   Frontend routes: /home, /about, /login (HTML pages)
 *   API routes: /api/students, /api/courses (JSON data)
 *   Prefix se dono alag. CORS, security rules easy to apply.
 * ============================================================
 */
@RequestMapping("/api/students")
public class StudentController {

    /*
     * @Autowired — Spring inject karega StudentService bean.
     * Controller → Service call. Controller mein LOGIC nahi likhna.
     * Controller ka kaam: request lo, service ko do, response bhejo. Bas.
     * NAHI LAGAYA TO: studentService = null → method call pe NullPointerException.
     */
    @Autowired
    private StudentService studentService;

    // ==========================================
    // POST — student create
    // ==========================================

    /*
     * ============================================================
     * @PostMapping
     *
     * YE KYA HAI: POST HTTP method handle karo. Naya resource CREATE.
     *   = @RequestMapping(method = RequestMethod.POST) ka shorthand.
     *   URL: POST /api/students
     *   Body: { "name": "Arpan", "email": "a@b.com", "age": 25 }
     *
     * KYUN LAGAYA: POST request aaye toh ye method chale. HTTP method mapping.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - POST /api/students request aayegi → Spring ko pata nahi kisko dena hai
     *   - 405 Method Not Allowed ya 404 Not Found
     *   - Client create nahi kar payega student ko
     *
     * REAL WORLD ANALOGY:
     *   @PostMapping = restaurant mein "NEW ORDER" counter.
     *   Customer naya order de → is counter pe jaaye.
     *   Counter nahi → customer confused "naya order kahan du?"
     * ============================================================
     */
    @PostMapping
    public ResponseEntity<StudentDTO> create(@Valid @RequestBody StudentDTO dto) {
        /*
         * ============================================================
         * @Valid — CRITICAL ANNOTATION
         *
         * YE KYA HAI: Request body ki VALIDATION trigger karo.
         *   Entity/DTO pe jo annotations lagaye (@NotBlank, @Email, @Min) — sab CHECK honge.
         *   Ek bhi fail → MethodArgumentNotValidException → GlobalExceptionHandler → 400 error.
         *
         * KYUN LAGAYA: Bina iske validation annotations SAB DEAD hain. Kuch nahi check hoga.
         *
         * NAHI LAGAYA TO KYA HOGA:
         *   - @NotBlank lagaya name pe → blank name bheja → SAVE HO JAYEGA! Validation DEAD.
         *   - @Email lagaya → "abc" bheja → accept ho jayega! No email check.
         *   - @Min(18) lagaya → age = 5 bheja → save ho jayega! No age check.
         *   - SAB VALIDATION ANNOTATIONS USELESS — @Valid ke bina ye decorations hain.
         *   - COMMON GALTI: Validation lagaye Entity pe, @Valid bhool gaye Controller mein.
         *
         * REAL WORLD ANALOGY:
         *   @Valid = SECURITY SCANNER airport pe.
         *   Rules toh hain (no liquids, no sharp objects = @NotBlank, @Email).
         *   Scanner ON hai (@Valid lagaya) → sab check hoga → violation pe REJECT.
         *   Scanner OFF (@Valid nahi lagaya) → rules hain but check nahi → sab pass.
         *
         * ============================================================
         * @RequestBody
         *
         * YE KYA HAI: HTTP request ka JSON body → Java object mein convert karo.
         *   Client ne bheja: { "name": "Arpan", "email": "a@b.com", "age": 25 }
         *   Jackson library: JSON parse → new StudentDTO() → setName("Arpan") → setEmail(...)
         *
         * KYUN LAGAYA: Bina iske Spring ko pata nahi ki JSON body ko Java object mein convert karna.
         *
         * NAHI LAGAYA TO KYA HOGA:
         *   - dto = null → NullPointerException on dto.toEntity()
         *   - Ya Spring default se URL parameters se try karega → fields null/empty
         *   - Client ne valid JSON bheja but server ko mila hi nahi → 400 Bad Request
         *
         * REAL WORLD ANALOGY:
         *   @RequestBody = TRANSLATOR (JSON → Java).
         *   Client ENGLISH (JSON) mein bola → translator (Jackson + @RequestBody) ne HINDI (Java object) mein convert kiya.
         *   Translator nahi → "kya bol raha hai? Samajh nahi aaya" → null/error.
         *
         * NOTE: Content-Type header "application/json" hona chahiye — nahi toh 415 Unsupported Media Type.
         * ============================================================
         */
        StudentDTO saved = studentService.addStudent(dto);
        return new ResponseEntity<>(saved, HttpStatus.CREATED);   // 201 Created
        /*
         * ============================================================
         * ResponseEntity<T>
         *
         * YE KYA HAI: HTTP response ka FULL CONTROL — body + status code + headers.
         *
         * KYUN LAGAYA: Proper HTTP status codes bhejne ke liye. REST API standard.
         *   HttpStatus.CREATED = 201 — naya resource bana.
         *
         * NAHI LAGAYA TO (plain object return kiya):
         *   - Hamesha 200 OK return hoga — client ko pata nahi ki create hua ya read
         *   - REST standard nahi follow → API consumers confused
         *   - Frontend mein status code pe logic hai → galat status → galat UI behavior
         *
         * REAL WORLD ANALOGY:
         *   ResponseEntity = COURIER ka TRACKING STATUS.
         *   "Delivered" (200) vs "Order Placed" (201) vs "Not Found" (404) — CLEAR status.
         *   Bina tracking: "kuch hua" — kya hua? Pata nahi.
         *
         * HTTP Status Codes (important ones):
         *   200 OK         — success (GET, PUT, DELETE)
         *   201 Created    — naya resource bana (POST)
         *   204 No Content — success, koi body nahi (DELETE sometimes)
         *   400 Bad Request — client ne galat data bheja (validation fail)
         *   401 Unauthorized — login nahi kiya (no token)
         *   403 Forbidden   — login kiya but permission nahi
         *   404 Not Found   — resource nahi mila
         *   500 Internal Server Error — server mein bug
         * ============================================================
         */
    }

    // ==========================================
    // GET — sab students with Pagination + Sorting
    // ==========================================

    /*
     * @GetMapping = @RequestMapping(method = GET) ka shorthand.
     * URL: GET /api/students?page=0&size=5&sort=name,asc
     * NAHI LAGAYA TO: GET request handle nahi hoga → 404.
     */
    @GetMapping
    public ResponseEntity<Page<StudentDTO>> getAll(Pageable pageable) {
        /*
         * Pageable — Spring AUTOMATICALLY URL se pagination parameters le leta.
         *   URL: GET /api/students?page=0&size=5&sort=name,asc
         *
         *   Spring internally:
         *     PageRequest.of(page=0, size=5, Sort.by("name").ascending())
         *     Tu ye object nahi banata — Spring URL dekhke auto banata.
         *
         *   HIBERNATE SQL:
         *     SELECT * FROM students ORDER BY name ASC LIMIT 5 OFFSET 0
         *     + SELECT COUNT(*) FROM students  (total count ke liye)
         *
         *   DEFAULT VALUES (agar URL mein nahi diya):
         *     page = 0 (first page), size = 20, sort = unsorted
         *
         *   BINA PAGINATION: findAll() → 10 lakh records → OutOfMemoryError.
         */
        return ResponseEntity.ok(studentService.getAllStudents(pageable));
        // ResponseEntity.ok() = status 200 + body
    }

    // ==========================================
    // GET — ek student by id
    // ==========================================

    /*
     * @GetMapping("/{id}") — "/{id}" = URL mein dynamic part.
     *   /api/students/5 → id = 5
     *   /api/students/99 → id = 99
     * NAHI LAGAYA TO: GET /api/students/5 handle nahi hoga → 404.
     */
    @GetMapping("/{id}")
    public ResponseEntity<StudentDTO> getById(@PathVariable Long id) {
        /*
         * ============================================================
         * @PathVariable
         *
         * YE KYA HAI: URL se value nikalo. Dynamic part of URL.
         *   URL: GET /api/students/5
         *   @PathVariable Long id → id = 5
         *
         * KYUN LAGAYA: URL mein {id} placeholder hai — uski value method parameter mein chahiye.
         *
         * NAHI LAGAYA TO KYA HOGA:
         *   - URL mein id hai but method ko milegi NAHI
         *   - Spring URL pattern match nahi kar payega → 404 ya error
         *   - id = null/0 → wrong query → wrong result ya exception
         *
         * REAL WORLD ANALOGY:
         *   @PathVariable = HOTEL ka ROOM NUMBER (URL mein).
         *   Guest bole "Room 305" → receptionist (Spring) "305" extract kare → us room mein le jaaye.
         *   @PathVariable nahi → receptionist bole "kaunsa room? Number toh batao!" → confused.
         *
         * @PathVariable vs @RequestParam:
         *   @PathVariable: URL ka HISSA     → /students/5       (resource identify karna)
         *   @RequestParam: URL ke BAAD ?    → /students?id=5    (filter/search)
         *   REST convention: resource id = PathVariable, filters = RequestParam.
         *
         * Naam match rule: {id} mein "id" → @PathVariable Long id mein "id" — naam SAME chahiye.
         *   Nahi toh: @PathVariable("id") Long studentId — explicit naam do.
         * ============================================================
         */
        return ResponseEntity.ok(studentService.getStudentById(id));
    }

    // ==========================================
    // PUT — update
    // ==========================================

    /*
     * @PutMapping("/{id}") — PUT request handle. Existing resource UPDATE.
     * URL: PUT /api/students/5
     * NAHI LAGAYA TO: PUT request handle nahi hoga → 405 Method Not Allowed.
     */
    @PutMapping("/{id}")
    public ResponseEntity<StudentDTO> update(@PathVariable Long id, @Valid @RequestBody StudentDTO dto) {
        /*
         * @PathVariable Long id → URL se student id (kaunsa update karna hai)
         * @Valid → validation check (updated data bhi valid hona chahiye)
         * @RequestBody StudentDTO dto → JSON body se updated data
         */
        return ResponseEntity.ok(studentService.updateStudent(id, dto));
    }

    // ==========================================
    // DELETE
    // ==========================================

    /*
     * @DeleteMapping("/{id}") — DELETE request handle. Resource HATAO.
     * URL: DELETE /api/students/5
     * NAHI LAGAYA TO: DELETE request handle nahi hoga → 405 ya 404.
     */
    @DeleteMapping("/{id}")
    public ResponseEntity<String> delete(@PathVariable Long id) {
        studentService.deleteStudent(id);
        return ResponseEntity.ok("Student deleted successfully");
    }

    // ==========================================
    // SEARCH — by name or age range
    // ==========================================

    /*
     * @GetMapping("/search") — search endpoint.
     * URL examples:
     *   GET /api/students/search?name=Arpan
     *   GET /api/students/search?minAge=20&maxAge=30
     */
    @GetMapping("/search")
    public ResponseEntity<List<StudentDTO>> search(
            @RequestParam(required = false) String name,
            @RequestParam(required = false) Integer minAge,
            @RequestParam(required = false) Integer maxAge) {
        /*
         * ============================================================
         * @RequestParam
         *
         * YE KYA HAI: URL ke ? ke baad ke parameters nikalo.
         *   URL: /search?name=Arpan&minAge=20
         *   name = "Arpan", minAge = 20
         *
         * KYUN LAGAYA: Search filters URL mein aate hain — unhe method mein lena hai.
         *
         * NAHI LAGAYA TO KYA HOGA:
         *   - URL mein parameters hain but method ko milenge NAHI
         *   - name, minAge, maxAge sab null rahenge → search kaam nahi karega
         *
         * required = false → nahi diya toh null (error nahi)
         * required = true (default) → nahi diya toh 400 Bad Request error
         * defaultValue = "10" → nahi diya toh "10" use karo
         *
         * Integer vs int (IMPORTANT):
         *   int minAge → null nahi ho sakta → required = false ka matlab nahi
         *   Integer minAge → null ho sakta → optional parameter correctly handle
         *
         * REAL WORLD ANALOGY:
         *   @RequestParam = SEARCH FILTERS on Amazon.
         *   "Shoes, Size 10, Color Black" → ?category=shoes&size=10&color=black
         *   Filters optional hain — nahi diya toh sab dikhao.
         * ============================================================
         */

        if (name != null) {
            return ResponseEntity.ok(studentService.searchByName(name));
        }
        if (minAge != null && maxAge != null) {
            return ResponseEntity.ok(studentService.searchByAgeRange(minAge, maxAge));
        }
        return ResponseEntity.badRequest().body(Collections.emptyList());
        // badRequest() = 400 — koi valid search parameter nahi diya
    }
}
