# SPOF (Single Point of Failure) — ek dabba gira to sab gira

> **NAV** — KYA: SPOF kya, kaise dhoondhe, kya karein. lagta hai: [banking](../SYSTEM_DESIGNS/13_banking_system/13_banking_system.md) · [file-upload](../SYSTEM_DESIGNS/07_file_upload_validate_system/07_file_upload.md) · [rate-limiter](../SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md)

## Koi poochhe to aise bolo

SPOF matlab ek aisa dabba jo gira to poora system gir jaaye. Use dhoondhne ke liye request ka raasta banao (user, LB, server, Redis, DB) aur har dabbe pe poochho "ye gira to kya hoga?". Ilaaj hai copy rakhna, aur copy alag jagah (alag AZ) rakhna. Cloud pe bhi machine roz girti hai, bas copy aur failover itne achhe hote hain ki user ko pata nahi chalta.

## Kahani (bank branch)

```
   10 cashier  ->  ek gira, baaki 9 chalte      = SPOF nahi
   1 register  ->  jala to 10 cashier bekaar    = SPOF
```

Branch me 10 cashier hain, matlab cashier ka SPOF nahi hai. Par register ek hi hai. Wo jala to 10 cashier hote hue bhi branch band. Yahi chhupa SPOF hai: diagram me sab double dikhta hai, par sab ek hi cheez pe tike hote hain.

Design me ise kaise dekhte: Redis, RDS, S3 jaisi managed cheez ki copy aur failover AWS khud sambhalta hai, isliye design bolte waqt "Redis gira to?" bolne ki zaroorat nahi, wo andar ka kaam hai. Par ek dabba kahin bhi akela nahi rakhna. Dhyaan wahan jahan dabba HUM khud rakhte hain: ek hi EC2 pe server chalaya aur wo gira = poora system down. Isliye server 3-4, LB ke peeche.

## Ek trap

Copy rakh di, par dono ek hi building (ek AZ) me rakhi. Aag lagi to dono saath jal gaye. Isliye copy hamesha alag jagah. Aur har SPOF hatana zaroori nahi hai: paise wale raaste (ledger, payment) pe multi-AZ zaroor rakho, admin report ek machine pe bhi chal jaayegi.
