package com.javatpoint.service;
import java.util.ArrayList;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

import com.javatpoint.model.User;
import com.javatpoint.repository.UserRepository;

// SERVICE (Spring Security bridge): tera User entity <-> Spring ka UserDetails.
// SecurityConfig isko inject karta; login pe Spring KHUD loadUserByUsername() call karta.
@Service
public class CustomUserDetailsService implements UserDetailsService {

    @Autowired
    private UserRepository userRepository; // Inject your user repository

    // Spring login pe ye call karta: email -> DB user -> Spring ke UserDetails me convert
    @Override
    public UserDetails loadUserByUsername(String email) throws UsernameNotFoundException {
        // Find user by email
        User user = userRepository.findByEmail(email);
        if (user == null) {
            throw new UsernameNotFoundException("User not found with email: " + email);
        }
        // tera User entity -> Spring Security ka UserDetails (email, hashed-password, roles)
        return new org.springframework.security.core.userdetails.User(
            user.getEmail(),
            user.getPassword(),
            new ArrayList<>()   // authorities/roles (khaali = role-based access nahi)
        );
    }
}
