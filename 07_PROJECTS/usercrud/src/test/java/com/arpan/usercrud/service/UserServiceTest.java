package com.arpan.usercrud.service;

import com.arpan.usercrud.exception.UserNotFoundException;
import com.arpan.usercrud.model.User;
import com.arpan.usercrud.repository.UserRepository;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

// static imports (assert/mockito) — TODO: use karte waqt yaad rakh
import static org.junit.jupiter.api.Assertions.*;      // assertEquals, assertThrows, assertTrue...
import static org.mockito.Mockito.*;                    // when, verify, times, never...

@ExtendWith(MockitoExtension.class)   // Mockito ko JUnit5 ke saath chalu
class UserServiceTest {

    @Mock                             // NAKLI repository (DB nahi)
    private UserRepository repository;

    @InjectMocks                      // asli service — upar wala mock isme inject ho jaata
    private UserService service;

    @Test
    void getById_whenUserExists_returnsUser() {
        User user = new User(1L, "Arpan", "x@x.com", "x", 25, "USER");
        when(repository.findById(1L)).thenReturn(Optional.of(user));

        User result = service.getById(1L);

        assertEquals(user, result);
        verify(repository).findById(1L);
    }

    @Test
    void getById_whenUserMissing_throws() {
        when(repository.findById(99L)).thenReturn(Optional.empty());
        assertThrows(UserNotFoundException.class, () -> service.getById(99L));
    }

    @Test
    void create_savesAndReturnsUser() {
        User user = new User(1L, "Arpan", "x@x.com", "x", 25, "USER");
        when(repository.save(user)).thenReturn(user);
        assertEquals(user, service.create(user));
        verify(repository).save(user);
    }

    @Test
    void getAll_returnsList() {
        User u1 = new User();
        u1.setId(1L); u1.setName("A");
        User u2 = new User();
        u2.setId(2L); u2.setName("B");
        List<User> users = new ArrayList<>();
        users.add(u1);
        users.add(u2);
        when(repository.findAll()).thenReturn(users);
        List<User> result = service.getAll();
        verify(repository).findAll();
        assertEquals(2,  result.size());
    }
}
