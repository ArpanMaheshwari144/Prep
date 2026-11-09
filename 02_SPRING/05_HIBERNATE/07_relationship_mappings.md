# Relationship Mappings

## Scene: usercrud ki do table

```
authors              books
id  name             id  title   author_id   <- FK, har kitaab pe "lekhak kaun"
1   Arpan            1   Java    1
2   Suresh           2   Spring  1
                     3   Python  2
```

Rishta DB me sirf ek jagah likha hai: `books.author_id`. Isliye Java me bhi do side ka kaam alag hai:

- **Book = likhne wali side.** FK (`author_id`) isi table me hai. Book me jo author set karoge, Hibernate DB me wahi likhega.
  ```java
  @ManyToOne @JoinColumn(name = "author_id")
  Author author;
  ```
- **Author = sirf dekhne wali list.** Author ki `books` list DB me kuch nahi likhti, bas Book table se padh ke dikhati hai.
  `mappedBy = "author"` ka matlab hai: "rishta Book ke `author` field me likha hai, mere paas nahi".
  ```java
  @OneToMany(mappedBy = "author")
  List<Book> books;
  ```

(Docs me inka naam owning side aur inverse side hai.)

## Dikkat

- `mappedBy` bhool gaya to Hibernate ise do alag rishte samajhta hai aur ek faaltu teesri table `author_books` bana deta hai.
- Sirf `author.getBooks().add(book)` kiya aur `book.setAuthor(author)` nahi kiya, to DB me `author_id` NULL jaata hai. Wajah ye hai ki
  Hibernate sirf likhne wali side (Book) dekhta hai, dekhne wali list ko nahi. Hal (cascade wali file jaisa):
  `addBook(b) { books.add(b); b.setAuthor(this); }`.

## Many-to-many (Student aur Course)

Beech me teesri table `student_course (student_id, course_id)` banti hai. `@ManyToMany` likhna aasaan hai, par asli kaam me beech wali table
pe extra cheez chahiye hoti hai (enrolledDate, grade). Wo `@ManyToMany` me nahi aa sakti, isliye `Enrollment` naam ki entity banao jisme do
`@ManyToOne` hon.

## Prod wala trap (usercrud me tha)

```java
@Entity @Data class Author { @OneToMany(mappedBy = "author") List<Book> books; }
@Entity @Data class Book   { @ManyToOne Author author; }
// log.info(author) -> Author.toString -> books -> Book.toString -> author -> ... StackOverflowError
```

`@Data` ne dono taraf `toString` bana diya. Author ne books chhapi, har book ne apna author, author ne phir books, aur ye chakkar tab tak
chala jab tak StackOverflowError nahi aaya. Entity pe `@Data` mat lagao. `@Getter @Setter` lagao, `toString` me rishte wala field mat daalo,
aur bahar bhejne ke liye DTO lo. AI bhi entity pe `@Data` laga deta hai, isliye PR me ye line pakadna tera kaam hai.
