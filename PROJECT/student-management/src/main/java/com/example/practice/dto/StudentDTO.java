package com.example.practice.dto;

/*
 * ============================================================
 * YE KYA HAI: DTO (Data Transfer Object) — client ko sirf zaroori fields bhej. Sensitive data chhupa.
 * KYUN CHAHIYE: Entity seedha client ko bhejoge toh password, internal fields LEAK ho jayenge.
 * KYA KARTA: Entity aur Client ke beech ek FILTER ki tarah kaam karta — safe data hi jaaye.
 * NAHI HUA TO: Client ko password, createdAt, internalFlags sab dikh jaayega — SECURITY RISK.
 *
 * REAL WORLD ANALOGY:
 *   Entity = tera FULL Aadhaar card (photo, address, fingerprint, sab).
 *   DTO    = sirf naam aur photo wala ID card — bahar dikhane ke liye safe.
 *   Tu bank mein Aadhaar deta, lekin bahar kisi ko sirf naam batata — DTO wahi karta.
 *
 *   Doosri analogy:
 *   Entity = tera KITCHEN (sab ingredients, recipes, mess sab hai).
 *   DTO    = SERVING PLATE (sirf clean, garnished dish client ko dikhti).
 *   Kitchen ka mess client ko nahi dikhana — plate mein saja ke do.
 *
 *   DB (Entity):  | id | name | email | age | password | createdAt | role |
 *   Client (DTO): | id | name | email | age |  <-- password NAHI! role NAHI!
 *
 * BONUS FAYDA (DECOUPLING):
 *   DB schema badla (10 naye columns add) → DTO same → API response same → client break nahi hoga.
 *   Ye = Decoupling. DB aur API independent. Ek badlo, doosra safe.
 * ============================================================
 */

import com.example.practice.entity.Student;
import lombok.*;
import java.io.Serializable;

/*
 * @Data — Lombok: getter, setter, toString, equals, hashCode auto generate.
 *   NAHI LAGAYA TO: Jackson JSON parse nahi kar payega (getter/setter chahiye), manually likhna padega.
 *
 * @NoArgsConstructor — empty constructor. Jackson ko CHAHIYE JSON → Object convert ke liye.
 *   NAHI LAGAYA TO: Jackson "Cannot construct instance" error dega — JSON deserialize FAIL.
 *
 * @AllArgsConstructor — full constructor. fromEntity() mein use hota — ek line mein object banao.
 *   NAHI LAGAYA TO: fromEntity() mein manually set karna padega har field.
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class StudentDTO implements Serializable {
    /*
     * ============================================================
     * Serializable KYUN CHAHIYE — Redis Cache ke liye ZAROORI:
     *
     * YE KYA HAI:
     *   Serializable = ek marker interface (koi method nahi, sirf TAG).
     *   Java ko batata: "Is class ke objects ko bytes mein convert kar sakta."
     *   Serialization = Object → bytes. Deserialization = bytes → Object.
     *
     * KYUN LAGAYA (Redis ke liye):
     *   Redis = external memory (RAM based cache). Network pe data jaata.
     *   Java Object seedha network pe nahi bhej sakte — bytes mein convert karna padta.
     *   Serializable implement kiya = Java ko permission diya: "Haan, isko convert kar sakta."
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   @Cacheable method call hoga →
     *   Spring try karega object ko Redis mein daalne →
     *   "org.springframework.data.redis.serializer.SerializationException:
     *    Cannot serialize" ERROR →
     *   Cache FAIL. Har request DB jaayegi. Performance DEAD.
     *
     * REAL WORLD ANALOGY:
     *   Tu COURIER bhej raha (Object bhej raha Redis ko).
     *   Courier company kehti: "Seedha cheez nahi le sakte. PACK karo pehle (serialize)."
     *     Object  = teri cheez (StudentDTO with name, age, email)
     *     Pack    = Serialization (Object → bytes/JSON)
     *     Courier = Network/Redis
     *     Unpack  = Deserialization (bytes/JSON → Object wapas)
     *   Bina packing (Serializable) = courier REJECT kare = "Cannot serialize" error.
     *
     *   Doosri analogy:
     *     Serializable = Object ka PASSPORT. Redis (foreign country) jaane ke liye ZAROORI.
     *     implements Serializable = passport ban gaya.
     *     Bina passport = border pe ruk jaayega = error.
     *
     * TECHNICAL NOTE:
     *   GenericJackson2JsonRedisSerializer (humara config) internally Jackson use karta,
     *   toh technically Serializable bina bhi JSON ban sakta. BUT:
     *     1. Best practice hai — Spring docs recommend karta.
     *     2. Agar koi JdkSerializer use kare toh bina Serializable = instant crash.
     *     3. Future mein serializer change kiya → Serializable pehle se hai → safe.
     *     4. Code padh ke CLEAR pata chalta — "ye class serialize hoga (cache/network ke liye)."
     *
     * INTERVIEW MEIN POOCHEIN:
     *   Q: DTO mein Serializable kyun lagaya?
     *   A: Redis cache ke liye. Object ko bytes/JSON mein convert karke network pe bhejte.
     *      Serializable = marker interface jo Java ko permission deta serialize karne ka.
     *      Bina iske SerializationException aata.
     * ============================================================
     */
    private Long id;
    private String name;
    private String email;
    private int age;
    // password NAHI hai — ye intentional. Client ko password kabhi nahi bhejte.

    /*
     * ============================================================
     * FACTORY METHOD PATTERN — fromEntity() aur toEntity()
     *
     * YE KYA HAI: Conversion methods — Entity <-> DTO convert karte hain.
     *
     * KYUN CHAHIYE: Entity seedha client ko NAHI bhejte (security risk).
     *   DB se Entity aaya → fromEntity() → DTO banao → client ko bhejo (safe).
     *   Client se DTO aaya → toEntity() → Entity banao → DB mein save karo.
     *
     * NAHI HUA TO KYA HOGA:
     *   - Entity seedha return karo → password, internal fields client ko dikh jayenge
     *   - Har jagah manually conversion likho → duplicate code, galti ka chance
     *   - Entity change kiya → sab jagah conversion update karo → NIGHTMARE
     *
     * REAL WORLD ANALOGY:
     *   fromEntity() = kitchen se dish aayi → WAITER plate mein saja ke customer ko de.
     *   toEntity()   = customer ne ORDER diya → waiter kitchen ko bata ke dish banwaye.
     *   Waiter (converter) ke bina: customer seedha kitchen mein ghuse → chaos!
     *
     * Real projects mein MapStruct ya ModelMapper library use hoti — auto mapping.
     * Abhi manually likh rahe — concept samajhne ke liye.
     * ============================================================
     */

    /*
     * ============================================================
     * fromEntity() — Entity → DTO convert
     *
     * YE KYA HAI: DB se data aaya (Entity) → client ke liye safe DTO banao.
     *
     * KYUN STATIC: StudentDTO.fromEntity(student) — bina object banaye call ho sake.
     *   StudentDTO dto = new StudentDTO(); dto.setName(...); — ye UGLY aur VERBOSE.
     *   StudentDTO dto = StudentDTO.fromEntity(student); — ye CLEAN aur READABLE.
     *
     * KAB USE: Service mein jab DB se Student entity aaya → client ko DTO bhejne se pehle.
     * ============================================================
     */
    public static StudentDTO fromEntity(Student student) {
        return new StudentDTO(
            student.getId(),
            student.getName(),
            student.getEmail(),
            student.getAge()
        );
    }

    /*
     * ============================================================
     * toEntity() — DTO → Entity convert
     *
     * YE KYA HAI: Client se data aaya (DTO) → DB ke liye Entity banao.
     *
     * KYUN INSTANCE METHOD (static nahi): dto.toEntity() — DTO object pe call.
     *   Iska data use karke Entity bana.
     *
     * KAB USE: Service mein jab client se DTO aaya → DB mein save karne se pehle.
     * NOTE: id SET nahi kar rahe — naya student hai toh DB auto-increment karega (@GeneratedValue).
     * ============================================================
     */
    public Student toEntity() {
        Student student = new Student();
        student.setName(this.name);
        student.setEmail(this.email);
        student.setAge(this.age);
        return student;
    }
}
