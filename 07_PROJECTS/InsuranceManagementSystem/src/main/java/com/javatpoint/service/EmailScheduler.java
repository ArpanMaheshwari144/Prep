package com.javatpoint.service;

import java.time.LocalDateTime;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;
import com.javatpoint.model.User;

import com.javatpoint.repository.UserRepository;



// SERVICE (scheduled batch-job): inactive users ko reminder-email. @Scheduled = time-based auto job (Spring chalata).
@Service
public class EmailScheduler {

	@Autowired
	UserRepository userRepository;

	@Autowired
	EmailService emailService;

	// @Scheduled(cron=...) -> abhi COMMENTED (job auto off). on karne pe cron-timing pe khud chalega.
	// kaam: X-time se inactive users dhoondho -> jise email nahi bheja (email_sent=false) usko bhejo -> flag true (duplicate rok = idempotency-jaisa)
//	@Scheduled(cron = "*/1 * * * * *")
	public void sendEmailToEligibleUsers(){
		LocalDateTime time24HoursAgo = LocalDateTime.now().minusMinutes(1);   // testing ke liye 1-min (naam "24Hours")
		List<User> users =userRepository.getUserLoggedIn24HoursAgo(time24HoursAgo);
		for (User obj: users) {
			if(!obj.getEmail_sent()) {                          // pehle nahi bheja tabhi bhejo
				emailService.sendPolicyClaimedEmail(obj.getEmail());
				obj.setEmail_sent(true);                        // dobara na jaaye
				userRepository.save(obj);
			}


		}


	}


}
