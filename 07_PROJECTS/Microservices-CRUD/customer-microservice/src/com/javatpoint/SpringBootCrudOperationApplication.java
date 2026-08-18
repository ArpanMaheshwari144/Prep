package com.javatpoint;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration;
import org.springframework.cloud.openfeign.EnableFeignClients;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.scheduling.annotation.EnableScheduling;

import com.feign.CustomerInterface;

@EnableFeignClients(clients = CustomerInterface.class)
@EnableScheduling
@ComponentScan  // Specify the base package for scanning
@SpringBootApplication(exclude = SecurityAutoConfiguration.class)
public class SpringBootCrudOperationApplication {
    public static void main(String[] args) {
        SpringApplication.run(SpringBootCrudOperationApplication.class, args);
    }
}
