# Checked vs unchecked exceptions

- **Checked:** compiler pehle hi rok deta hai, "handle karo ya `throws` likho". Ye bahar ki cheezon ke liye hain jo fail ho sakti
  hain, jaise file, DB, network (`IOException`, `SQLException`).
- **Unchecked:** compiler kuch nahi kehta, ye chalte waqt phatti hain. Ye code ki galti ke liye hain, jaise null ya galat index
  (`NullPointerException`, `IllegalArgumentException`). Pehchaan: `RuntimeException` ka bachcha.

## Tera bank

`InsufficientFundsException extends RuntimeException` hai, matlab unchecked. Isliye `withdraw()` ke signature me `throws` nahi
likhna padta, aur use bulane wale har method pe zabardasti `try-catch` nahi lagana padta. Usercrud ka `UserNotFoundException`
bhi aisa hi hai. Business wali galtiyon ke liye yahi aam tareeka hai.

## Prod wala trap

Spring ka `@Transactional` by default sirf unchecked exception pe rollback karta hai. Transfer me pehle Ravi ke 500 kate, phir
receipt PDF banate waqt `IOException` (checked) aaya. Spring ne rollback nahi kiya, commit kar diya, matlab Ravi ka paisa kat
gaya aur aage gaya hi nahi. Ilaaj: `@Transactional(rollbackFor = Exception.class)`, ya apni exception `RuntimeException` se
banao. (Ye Spring ka likha hua default niyam hai, Spring app chala ke nahi dekha.)
