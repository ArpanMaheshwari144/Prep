package com.example.practice.config;

/*
 * ============================================================
 * YE KYA HAI: Swagger Config — API documentation AUTOMATICALLY generate. Browser mein sab APIs dikhta.
 * KYUN CHAHIYE: Frontend dev ko APIs batao bina manually document likhe. Testing bhi seedha browser se.
 * KYA KARTA: springdoc library se sab @RestController scan → UI page generate.
 * NAHI HUA TO: Frontend dev bole "API kya hai?" → tu manually document likho. Update bhool gaya → outdated.
 *
 * REAL WORLD ANALOGY:
 *   Bina Swagger: Restaurant ka MENU NAHI hai. Waiter se ek ek dish poocho → slow, confusing.
 *   With Swagger: Menu card mil gaya — sab dishes (APIs), price (input/output), "try it out" button (test).
 *
 * URL: http://localhost:8080/swagger-ui.html
 *   Sab APIs ek page pe. Har API ka input/output. "Try it out" button se test.
 *   Postman ki zaroorat nahi — browser mein seedha test.
 *
 * SecurityConfig mein: /swagger-ui/**, /v3/api-docs/** → permitAll()
 *   Bina JWT access — documentation open honi chahiye developers ke liye.
 * ============================================================
 */

import io.swagger.v3.oas.models.OpenAPI;
import io.swagger.v3.oas.models.info.Info;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/*
 * ============================================================
 * @Configuration
 *
 * YE KYA HAI: Spring config class — @Bean methods define kar sakte.
 *
 * KYUN LAGAYA: OpenAPI bean banana hai → @Bean method chahiye → @Configuration class chahiye.
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - @Bean method detect nahi hoga → OpenAPI bean nahi banega
 *   - Swagger DEFAULT settings se chalega — title "OpenAPI definition", version "v0"
 *   - Kaam toh karega but customized nahi hoga — generic dikhega
 *
 * IMPORTANT NOTE: Ye config OPTIONAL hai!
 *   Bina ye class ke bhi swagger kaam karega — springdoc dependency add karo, auto scan hoga.
 *   Ye sirf CUSTOMIZATION ke liye hai (title, version, description set karna).
 *
 * REAL WORLD ANALOGY:
 *   @Configuration + @Bean = restaurant ka menu DESIGN customize karna.
 *   Bina config: plain text menu (default swagger — kaam karta but boring).
 *   With config: designer menu with restaurant name, logo, description (customized swagger).
 * ============================================================
 */
@Configuration
public class SwaggerConfig {

    /*
     * ============================================================
     * @Bean — OpenAPI object Spring bean ke roop mein
     *
     * YE KYA HAI: Swagger page ka metadata set karo — title, version, description.
     *
     * KYUN LAGAYA: API documentation professional dikhni chahiye.
     *   Default mein "OpenAPI definition" likha aata — ye GENERIC hai.
     *   Custom title lagao → "Student Management API" — CLEAR.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Swagger page kaam karega — but title "OpenAPI definition", version "v0"
     *   - Professional nahi dikhega — but FUNCTIONAL hai
     *   - Ye sirf cosmetic change hai — functionality pe koi farak nahi
     *
     * DEPENDENCY: springdoc-openapi-starter-webmvc-ui (pom.xml mein add karo)
     *   Add karo → auto scan → /swagger-ui.html ready. ZERO code se bhi swagger milta.
     *   Ye config sirf page customize karne ke liye.
     *
     * OPENAPI 3.0 — industry standard API specification.
     *   Swagger = UI tool (visual page banata).
     *   OpenAPI = specification (standard format mein API describe).
     *   Swagger OpenAPI spec se documentation generate karta.
     * ============================================================
     */
    @Bean
    public OpenAPI customOpenAPI() {
        return new OpenAPI()
                .info(new Info()
                        .title("Student Management API")
                        .version("1.0")
                        .description("Spring Boot REST API with JWT Security — by Arpan"));
    }
}
