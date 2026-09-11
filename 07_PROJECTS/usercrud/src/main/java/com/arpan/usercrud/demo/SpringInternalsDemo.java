package com.arpan.usercrud.demo;

import com.arpan.usercrud.service.UserService;
import jakarta.annotation.PostConstruct;
import org.jspecify.annotations.Nullable;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
@RestController
public class SpringInternalsDemo {

    private UserService userService;
    private ApplicationContext applicationContext;
    public SpringInternalsDemo(UserService userService, ApplicationContext applicationContext) {
        this.userService = userService;
        this.applicationContext = applicationContext;
    }

    @GetMapping("/internals")
    public String showInternals() {

        System.out.println(userService.getClass().getName());

        Dependency obj1 = applicationContext.getBean(Dependency.class);
        Dependency obj2 = applicationContext.getBean(Dependency.class);
        System.out.println(obj1 == obj2);


        System.out.println(applicationContext.getBean(DemoConfig.class).getClass().getName());
        Dependency directDep = applicationContext.getBean(Dependency.class);
        Dependency insideDep = applicationContext.getBean(SomeService.class).getDependency();
        System.out.println(directDep == insideDep);


        DummyBeam p1 = applicationContext.getBean(DummyBeam.class);
        DummyBeam p2 = applicationContext.getBean(DummyBeam.class);
        System.out.println(p1 == p2);


        System.out.println(applicationContext.getBeanDefinitionCount());

        String[] beanNames = applicationContext.getBeanDefinitionNames();
//        for (String beanName : beanNames) {
//            System.out.println(beanName);
//        }
        return beanNames[0];
    }
}


@Component
class LifecycleBean implements InitializingBean{
    public LifecycleBean(){
        System.out.println("1. constructor");
    }

    private UserService userService;

    @Autowired
    public void setUserService(UserService userService) {
        this.userService = userService;
        System.out.println("2. dependency injected");
    }


    @PostConstruct
    public void init(){
        System.out.println("3. PostConstruct (ready)");
    }


    @Override
    public void afterPropertiesSet() throws Exception {
        System.out.println("4. afterPropertiesSet");
    }
}

@Configuration
//@Configuration(proxyBeanMethods = false)   // <- ye uncomment (upar wala comment) karke run -> inter-bean FALSE + DemoConfig plain (no $$SpringCGLIB$$)
class DemoConfig {
    @Bean
    Dependency dependency() {
        return new Dependency();
    }

    @Bean
    SomeService someService() {
        return new SomeService(dependency());
    }

}

class Dependency{

}

class SomeService {
    private Dependency dependency;
    public SomeService(Dependency d) {
        this.dependency = d;
    }

    public Dependency getDependency() {
        return dependency;
    }
}


@Component
class LoggingBeanPostProcessor implements BeanPostProcessor {
    @Override
    public @Nullable Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
        System.out.println(bean.getClass().getName());
        return bean;
    }

    @Override
    public @Nullable Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        System.out.println(bean.getClass().getName());
        return bean;
    }
}

@Component
@Scope("prototype")
class DummyBeam{

}
