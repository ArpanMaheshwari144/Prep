package com.javatpoint.service;

import com.javatpoint.model.User;

import java.time.LocalDateTime;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Service;

import com.javatpoint.repository.UserRepository;

// SERVICE: OTP / forgot-password flow. 3 step -> (1) OTP bhejo (2) verify (3) reset password.
@Service
public class OtpService {

	@Autowired
	private JavaMailSender javaMailSender;

	@Autowired
	private UserRepository userRepository;

	 @Autowired
	 private BCryptPasswordEncoder passwordEncoder;

	// OTP ko email pe bhejo (plain-text mail)
	public void sendOtpEmail(String toEmail, String Otp) {
		SimpleMailMessage message = new SimpleMailMessage();
		message.setTo(toEmail);
		message.setSubject("You Otp Send");
		message.setText("Your new password " + Otp);
		javaMailSender.send(message);
	}

	// STEP 1: user dhoondho -> OTP banao -> user pe OTP + 20-min EXPIRY set -> save -> email. (na mile to exception)
	public void generateAndSendOtp(String email) {
		User user = userRepository.findByEmail(email);
		if (user != null) {
			String otp = generateOTP();
			user.setOtp(otp);
			user.setOtp_expired_time(LocalDateTime.now().plusMinutes(20));   // 20-min tak valid (TTL-jaisa)
			userRepository.save(user);
			sendOtpEmail(email, otp);
		} else {
			throw new RuntimeException("User not found");
		}
	}

	// random 4-digit (1000-9999)
	public String generateOTP() {
		int otp = (int) (Math.random() * 9000) + 1000;
		return String.valueOf(otp);
	}

	// STEP 2: OTP match AUR abhi valid (expire nahi hua) -> true
	public boolean verifyOtp(String email,String otp) {
		User user = userRepository.findByEmail(email);
		if (user!=null && user.getOtp().equals(otp) &&
				user.getOtp_expired_time().isAfter(LocalDateTime.now())) {
			return true;
		}
		return false;
	}

	// STEP 3: naya password BCrypt-hash karke set -> OTP null (one-time-use, replay rok) -> save
	public void resetPassword(String email, String newPassword) {
		User user = userRepository.findByEmail(email);
		if(user!=null) {
			String hashedPassword=passwordEncoder.encode(newPassword);
			user.setPassword(hashedPassword);
			user.setOtp(null);
			user.setOtp_expired_time(null);
			userRepository.save(user);
		}

	}

}
