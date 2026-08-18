package com.javatpoint.model;

import java.sql.Timestamp;
import java.time.LocalDateTime;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;



// ENTITY -> "user" table. Auth user (username/email/password + verification-token/OTP/enabled). login/signup ka base.
@Entity
@Table(name = "user")
public class User {
	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Column(name = "userid")
	private int userid;
	
	@Column(name = "username")
	private String username;
	
	@Column(name = "password")
	private String password;
	
	@Column(name = "email")
	private String email;
	
	@Column(name = "email_sent")
	private boolean email_sent;
	
	@Column(name = "last_login")
	private Timestamp last_login;
	
	@Column(name = "otp")
	private String otp;
	
	@Column(name = "otp_expired_time")
	private LocalDateTime otp_expired_time ;
	
	@Column(name = "verification_token")
	private String verification_token;
	
	@Column(name = "verification_token_expired_time")
	private LocalDateTime verification_token_expired_time;
	
	@Column(name = "is_enabled")
	private boolean is_enabled=false;
	
	

	public User() {
		super();
		// TODO Auto-generated constructor stub
	}

	public User(int userid, String username, String email,String password,
			String otp,LocalDateTime otp_expired_time,
			String verification_token,LocalDateTime verification_token_expired_time,boolean is_enabled) {
		super();
		this.userid = userid;
		this.username = username;
		this.password = password;
		this.email = email;
		this.otp=otp;
		this.otp_expired_time=otp_expired_time;
		this.verification_token=verification_token;
		this.verification_token_expired_time=verification_token_expired_time;
		this.is_enabled=is_enabled;
	}
	public User(int userid, String username, String email,String password) {
		super();
		this.userid = userid;
		this.username = username;
		this.password = password;
		this.email = email;
		
	}
	

	public int getUserId() {
		return userid;
	}

	public void setUserId(int userid) {
		this.userid = userid;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public boolean getEmail_sent() {
		return email_sent;
	}

	public void setEmail_sent(boolean email_sent) {
		this.email_sent = email_sent;
	}

	public Timestamp  getLast_login() {
		return last_login;
	}

	public void setLast_login(Timestamp timestamp) {
		this.last_login = timestamp;
	}

	public String getOtp() {
		return otp;
	}

	public void setOtp(String otp) {
		this.otp = otp;
	}

	public LocalDateTime getOtp_expired_time() {
		return otp_expired_time;
	}

	public void setOtp_expired_time(LocalDateTime otp_expired_time) {
		this.otp_expired_time = otp_expired_time;
	}

	public String getVerification_token() {
		return verification_token;
	}

	public void setVerification_token(String verification_token) {
		this.verification_token = verification_token;
	}

	public LocalDateTime getVerification_token_expired_time() {
		return verification_token_expired_time;
	}

	public void setVerification_token_expired_time(LocalDateTime verification_token_expired_time) {
		this.verification_token_expired_time = verification_token_expired_time;
	}

	public boolean isIs_enabled() {
		return is_enabled;
	}

	public void setIs_enabled(boolean is_enabled) {
		this.is_enabled = is_enabled;
	}
	
    
//	
	
	
	
	
	
	
	

}
