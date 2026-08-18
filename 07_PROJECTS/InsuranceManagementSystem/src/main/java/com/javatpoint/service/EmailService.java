package com.javatpoint.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.stereotype.Service;

import com.javatpoint.model.User;

// SERVICE: actual EMAIL bhejna (JavaMailSender = Spring ka SMTP mail tool; config application.properties me).
@Service
public class EmailService {

	@Autowired
	private JavaMailSender javaMailSender;

	// reminder email (EmailScheduler se call). SimpleMailMessage = plain-text mail (to+subject+text -> send).
	public void sendPolicyClaimedEmail(String toEmail) {
		SimpleMailMessage message = new SimpleMailMessage();
		message.setTo(toEmail);
		message.setSubject("Policy Claimed ");
	    message.setText("FATAL - Application crash. Save your job !!");   // placeholder text
	    javaMailSender.send(message);
	}

	// signup verification: token-wala /verify link banao -> email bhejo -> user click kare -> email verify
	public void sendVerificationEmail(User user) {
		String verificationurl="http://localhost:8081/verify?token="+user.getVerification_token();
		String message="Please verify your email by clicking the following link:"+verificationurl;
		SimpleMailMessage mail_message = new SimpleMailMessage();
		mail_message.setTo(user.getEmail());
		mail_message.setSubject("Email verification");
		mail_message.setText(message);
	    javaMailSender.send(mail_message);
	}

}
