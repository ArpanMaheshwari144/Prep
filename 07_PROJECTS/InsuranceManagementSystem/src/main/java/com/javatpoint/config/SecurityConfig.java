package com.javatpoint.config;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;

// SECURITY LAYER (Spring Security config). Teen kaam: (1) password HASH (2) AUTHENTICATE kaise (DB se)
// (3) kaun-sa URL public vs protected. Old-style (WebSecurityConfigurerAdapter, Spring-Security-5).
@EnableWebSecurity              // Spring Security ON -> filter-chain lag jaati
@Configuration
public class SecurityConfig extends WebSecurityConfigurerAdapter {

    @Autowired
    private UserDetailsService userDetailsService;   // custom: DB se user laata (email -> user + hash + roles)

    // Password kabhi plain store nahi -> BCrypt hash. (slow-by-design + salt -> brute-force mushkil)
    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    // AUTHENTICATION: user DB se laao (userDetailsService) + password usi BCrypt se compare
    // (dono jagah SAME encoder -> warna match kabhi nahi hoga)
    @Override
    protected void configure(AuthenticationManagerBuilder auth) throws Exception {
        auth
            .userDetailsService(userDetailsService)
            .passwordEncoder(passwordEncoder());
    }

    // AUTHORIZATION: kaun-sa URL bina-login khule, kaun-sa protected
    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http
            .csrf().disable()                        // REST/stateless -> form-CSRF token nahi chahiye
            .authorizeRequests()
            // ye endpoints PUBLIC (login/signup/OTP/password-change) -> warna user login hi kaise kare
            .antMatchers("/login", "/save","/change/password","/verify/otp","/request/otp").permitAll()
            .anyRequest().authenticated()            // baaki SAB pe login zaroori
            .and()
            .httpBasic();                            // simple Basic-Auth (username:password header). JWT nahi is project me.
    }

    // AuthenticationManager ko BEAN banaya -> login / OTP-verify me manually authenticate kar sako
    @Bean
    @Override
    public AuthenticationManager authenticationManagerBean() throws Exception {
        return super.authenticationManagerBean();
    }
}
