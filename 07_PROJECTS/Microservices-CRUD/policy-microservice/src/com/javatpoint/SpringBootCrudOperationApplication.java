package com.javatpoint;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.scheduling.annotation.EnableScheduling;

@EnableScheduling
@ComponentScan
@SpringBootApplication(exclude = SecurityAutoConfiguration.class)
public class SpringBootCrudOperationApplication 
{
public static void main(String[] args) 
{
SpringApplication.run(SpringBootCrudOperationApplication.class, args);
}
}


//server.port=8081
//spring.datasource.url=jdbc:mysql://localhost:3306/insurance
//spring.datasource.username=root
//spring.datasource.password=sudarshan
//spring.jpa.hibernate.ddl-auto=update
//spring.jpa.show-sql=true
//spring.jpa.properties.hibernate.format_sql=true
//spring.jpa.properties.hibernate.dialect=org.hibernate.dialect.MySQL5Dialect
//spring.jpa.hibernate.ddl-auto=update
