package com.javatpoint.serviceImpl;

import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.util.Optional;
import java.util.UUID;
import java.util.regex.Pattern;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

import com.javatpoint.DTO.LoginDTO;
import com.javatpoint.DTO.UserDTO;
import com.javatpoint.model.LoginMessage;
import com.javatpoint.model.User;
import com.javatpoint.repository.UserRepository;
import com.javatpoint.service.EmailService;
import com.javatpoint.service.UserService;

// SERVICE IMPL: UserService interface ka logic. User auth -- signup/verify/login/password.
@Service
public class UserServiceImpl implements UserService {

	@Autowired
	UserRepository userRepository;

	@Autowired
	PasswordEncoder passwordEncoder;   // BCrypt

	@Autowired
	EmailService emailService;

	private static final String EMAIL_REGEX = "^[A-Za-z0-9+_.-]+@(.+)$";

	// SIGNUP: pass HASH + dup-check + email-valid + token(24h) + disabled + verify-mail
	@Override
	public LoginMessage addUser(UserDTO userdto) {
		User user = new User(
				userdto.getUserid(),
				userdto.getUsername(),
				userdto.getEmail(),
				this.passwordEncoder.encode(userdto.getPassword())   // password HASH

		);
		User existuser = userRepository.findByEmail(userdto.getEmail());
		if (existuser != null) {
			if (existuser.getEmail().equals(user.getEmail())) {
				return new LoginMessage("email already  exist", false);   // duplicate email
			}

		}
		if (!isValidEmail(user.getEmail())) {
			return new LoginMessage("envalid email", false);              // format galat
		}
		String token = UUID.randomUUID().toString();                     // verification token
		user.setVerification_token(token);
		user.setVerification_token_expired_time(LocalDateTime.now().plusHours(24));  // 24h valid
		user.setIs_enabled(false);                                        // verify tak login-block
		emailService.sendVerificationEmail(user);                         // link email
		userRepository.save(user);
		return new LoginMessage("Login succes", true);
	}

	// LOGIN: email se user -> BCrypt matches(plain, hash) [NA equals] -> success/fail
	@Override
	public LoginMessage loginUser(LoginDTO logindto) {
		String message = "";
		User user1 = userRepository.findByEmail(logindto.getEmail());
		if (user1 != null) {
			String password = logindto.getPassword();
			String encodedPassword = user1.getPassword();
			Boolean isPasswordRight = passwordEncoder.matches(password, encodedPassword);   // hash-compare
			if (isPasswordRight) {
				Optional<User> user = userRepository.findOneByEmailAndPassword(logindto.getEmail(), encodedPassword);
				if (user.isPresent()) {
					return new LoginMessage("Login succes", true);
				} else {
					return new LoginMessage("Login Failed", false);
				}

			} else {
				return new LoginMessage("Password did not Match", false);
			}
		} else {
			return new LoginMessage("email does not exist", false);

		}
	}

	// last_login set + email_sent=false (reminder reset). Sirf ENABLED user.
	public boolean updateLoginTime(String email) {
		Optional<User> user1 = Optional.ofNullable(userRepository.findByEmail(email));
		if (user1.isPresent() && user1.get().isIs_enabled() == true) {
			User user = user1.get();
			user.setLast_login(Timestamp.valueOf(LocalDateTime.now()));
			user.setEmail_sent(false);
			userRepository.save(user);
			return true;

		}
		return false;

	}

	@Override
	public User findByVerificationToken(String token) {
		return userRepository.findByVerificationToken(token);   // token -> user
	}

	// regex email-format check (signup me use)
	public boolean isValidEmail(String email) {
		Pattern pattern = Pattern.compile(EMAIL_REGEX);
		return pattern.matcher(email).matches();
	}

	// VERIFY: token null ya expire -> false; warna is_enabled=TRUE + token=null (one-time)
	@Override
	public boolean verifyEmail(String token) {
		User user = userRepository.findByVerificationToken(token);
		if (user == null || user.getVerification_token_expired_time().isBefore(LocalDateTime.now())) {
			return false;                                   // galat ya expire
		}
		user.setIs_enabled(true);                           // account active
		user.setVerification_token(null);                   // one-time -> clear
		user.setVerification_token_expired_time(null);
		userRepository.save(user);
		return true;
	}

	// naya password BCrypt-hash -> save
	@Override
	public boolean updatePassword(String email, String newRawPassword) {
		User user = userRepository.findByEmail(email);
		if (user == null) {
			return false;
		}
		user.setPassword(passwordEncoder.encode(newRawPassword));   // hash
		userRepository.save(user);
		return true;
	}

}
