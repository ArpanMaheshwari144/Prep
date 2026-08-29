package com.javatpoint.controller;

import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.javatpoint.DTO.LoginDTO;
import com.javatpoint.DTO.PasswordChangeRequest;
import com.javatpoint.DTO.UserDTO;
import com.javatpoint.model.LoginMessage;
import com.javatpoint.service.OtpService;
import com.javatpoint.service.UserService;

// CONTROLLER: auth ka poora HTTP-darwaza. 4 flow -- SIGNUP+verify · LOGIN · FORGOT-PASSWORD (OTP 3-step) · CHANGE-PASSWORD.
// Ye THIN layer: request pakdo -> service ko do -> ResponseEntity (body + HTTP status) wapas. Asli logic UserService/OtpService me.
//   @RestController   -> methods ka return seedha JSON response banta.
//   @CrossOrigin("*") -> koi bhi domain (React frontend) call kar sake.
//   ResponseEntity    -> body ke saath HTTP STATUS bhi (200 ok / 400 / 401 / 404) -> frontend ko saaf pata.
@RestController
@CrossOrigin(origins = "*")
public class AuthController {

	@Autowired
	UserService userService;

	@Autowired
	OtpService otpService;

	@Autowired
	AuthenticationManager authenticationManager;   // Spring ka login-verifier (SecurityConfig se aaya)

	// SIGNUP: UserDTO (JSON body) -> addUser (password-hash + verification-token + disabled + mail bhejo) -> 200 ok.
	@PostMapping("/save")
	public ResponseEntity<?> saveUser(@RequestBody UserDTO userdto) {
		LoginMessage message = userService.addUser(userdto);
		return ResponseEntity.ok(message);
	}

	// LOGIN: pehle updateLoginTime (user verified/enabled hai?) -> phir loginUser (BCrypt match).
	//   msg=false (user na mila / disabled) -> 400. warna login-result 200 me.
	@PostMapping("/login")
	public ResponseEntity<?> saveLogin(@RequestBody LoginDTO logindto) {
		boolean msg = userService.updateLoginTime(logindto.getEmail());
		LoginMessage message = userService.loginUser(logindto);
		if (msg) {
			return ResponseEntity.ok(message);
		} else {
			return ResponseEntity.status(HttpStatus.BAD_REQUEST).body("user not found");
		}
	}

	// FORGOT-PASSWORD  STEP 1/3: email (body Map se -- sirf 1 field, DTO ki zaroorat nahi)
	//   -> OTP generate + email bhejo. user na mila -> catch -> 400.
	@PostMapping("/request/otp")
	public ResponseEntity<?> requestOtp(@RequestBody Map<String, String> request) {
		String email = request.get("email");
		try {
			otpService.generateAndSendOtp(email);
			return ResponseEntity.ok("Password sent to your email.");
		} catch (Exception e) {
			return ResponseEntity.status(HttpStatus.BAD_REQUEST).body(e.getMessage());
		}
	}

	// FORGOT-PASSWORD  STEP 2/3: OTP + expiry check -> sahi=200 | galat/expire = 401 (Unauthorized).
	@PostMapping("/verify/otp")
	public ResponseEntity<String> verifyOtp(@RequestBody Map<String, String> request) {
		String email = request.get("email");
		String otp = request.get("otp");
		if (otpService.verifyOtp(email, otp)) {
			return ResponseEntity.ok("OTP verified. You can now reset your password.");
		} else {
			return ResponseEntity.status(HttpStatus.UNAUTHORIZED).body("Invalid or expired OTP.");
		}
	}

	// FORGOT-PASSWORD  STEP 3/3: naya password set (service me BCrypt-hash + OTP clear -> one-time).
	@PostMapping("/reset/password")
	public ResponseEntity<String> resetPassword(@RequestBody Map<String, String> request) {
		String email = request.get("email");
		String newPassword = request.get("newPassword");
		otpService.resetPassword(email, newPassword);
		return ResponseEntity.ok("Password reset succesfully");
	}

	// EMAIL VERIFY (signup ka doosra hissa): email me bheje link pe click -> ?token=.. (@RequestParam)
	//   -> verifyEmail: valid+not-expired -> account ENABLE + token clear. galat -> 400.
	@GetMapping("/verify")
	public ResponseEntity<String> verifyEmail(@RequestParam String token) {
		if (userService.verifyEmail(token)) {
			return ResponseEntity.ok("Email Veriefied SuccessFully");
		}
		return ResponseEntity.status(HttpStatus.BAD_REQUEST).body("Invalid or Expired Token");
	}

	// CHANGE-PASSWORD (logged-in user, forgot se ALAG) -- 3 check:
	//   (1) new == confirm? nahi -> 400.
	//   (2) ★ CURRENT password sach me sahi? -> AuthenticationManager.authenticate(email, currentPass)
	//       (Spring khud DB+BCrypt se verify karta -- yahi is file ki asli baat). galat -> 401.
	//   (3) updatePassword (naya hash) -> user na mila -> 404, warna 200.
	@PostMapping("/change/password")
	public ResponseEntity<String> changePassword(@RequestBody PasswordChangeRequest req) {
		if (!req.getNewPassword().equals(req.getConfirmPassword())) {
			return ResponseEntity.badRequest().body("New password and confirm password did not match");
		}
		try {
			Authentication auth = authenticationManager.authenticate(
					new UsernamePasswordAuthenticationToken(req.getEmail(), req.getCurrentPassword()));  // current-pass verify
			if (!auth.isAuthenticated()) {
				return ResponseEntity.status(HttpStatus.UNAUTHORIZED).body("Invalid current password");
			}
			if (!userService.updatePassword(req.getEmail(), req.getNewPassword())) {
				return ResponseEntity.status(HttpStatus.NOT_FOUND).body("User not found");
			}
			return ResponseEntity.ok("Password successfully updated");
		} catch (Exception e) {
			e.printStackTrace();
			return ResponseEntity.status(HttpStatus.UNAUTHORIZED).body("Authentication failed");
		}
	}
}
