# Relationship Mappings

## Koi poochhe "Author-Book ka rishta / mappedBy kya hai" to aise bolo

```
"Is kitaab ka lekhak kaun" DB me sirf ek jagah likha hai: books table ka author_id.
Isliye DB me wahi jaata hai jo kitaab ko bataya (book.setAuthor). Author ki books list sirf padhne ke liye hai,
usme add / remove karo to DB ko farak nahi padta.
mappedBy = "author" ka matlab: meri list mat dekho, rishta Book ke author me likha hai.
```

## Do table

```
authors              books
id  name             id  title   author_id    <- rishta sirf yahan likha hai
1   Arpan
2   Suresh
```

`authors` table me kitaabon ka koi khaana hi nahi hai.

```java
class Book   { @ManyToOne @JoinColumn(name = "author_id") Author author; }   // ye DB me likhta hai
class Author { @OneToMany(mappedBy = "author") List<Book> books; }          // ye sirf padhta hai
```

## Chala ke dekha (Boot 4 + Hibernate 7): har kadam pe books table

```
Kadam 1: java.setAuthor(arpan)          [kitaab ko bataya]
     books: Java    author_id=1          <- DB me likh gaya

Kadam 2: arpan.getBooks().add(spring)   [sirf Arpan ki list me daala]
     books: Spring  author_id=NULL       <- DB me kuch nahi likha

Kadam 3: arpan.getBooks().remove(java)  [Arpan ki list se nikala]
     books: Java    author_id=1          <- DB me kuch nahi badla

Kadam 4: java.setAuthor(suresh)         [kitaab ko naya lekhak bataya]
     books: Java    author_id=2          <- DB badal gaya
```

List list hai: usme add / delete karo, DB ko farak nahi padta. DB sirf kitaab ki taraf dekhta hai.

Isliye rishta jodna ho to hamesha `book.setAuthor(...)`. List bhi saath me theek rakhni ho to ek helper jo dono kaam kare:

```java
void addBook(Book b) { books.add(b); b.setAuthor(this); }
```

(Cascade wali file me `ghadi order_id=NULL` isi wajah se aaya tha.)

## Dikkat: mappedBy likhna bhool gaye

Chala ke dekha:

```
mappedBy ke bina  -> DB me bani table: [AUTHOR_BOOKS, BOOK, AUTHOR]   <- faaltu teesri table
mappedBy ke saath -> DB me bani table: [BOOK, AUTHOR]
```

Hibernate ko laga list ka apna alag rishta hai, to usne list ke liye alag join table `author_books` bana di. Ab rishta do jagah likha jaata
hai, aur dono alag baat bol sakte hain.

## Many-to-many (Student aur Course)

Beech me teesri table `student_course (student_id, course_id)` banti hai. `@ManyToMany` likhna aasaan hai, par asli kaam me beech wali table
pe extra cheez chahiye hoti hai (enrolledDate, grade). Wo `@ManyToMany` me nahi aa sakti, isliye `Enrollment` naam ki entity banao jisme do
`@ManyToOne` hon.

## Prod wala trap (usercrud me tha)

```java
@Entity @Data class Author { @OneToMany(mappedBy = "author") List<Book> books; }
@Entity @Data class Book   { @ManyToOne Author author; }
// log.info(author) -> StackOverflowError   (chala ke dekha)
```

`@Data` ne dono taraf `toString` bana diya. Author ne books chhapi, har book ne apna author, author ne phir books, aur ye chakkar tab tak
chala jab tak StackOverflowError nahi aaya. Entity pe `@Data` mat lagao. `@Getter @Setter` lagao, `toString` me rishte wala field mat daalo,
aur bahar bhejne ke liye DTO lo. AI bhi entity pe `@Data` laga deta hai, isliye PR me ye line pakadna tera kaam hai.
