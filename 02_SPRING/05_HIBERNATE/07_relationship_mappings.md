# Relationship Mappings

## Kahani: kitaab ke peeche ki mohar

Library me har kitaab ke peeche mohar: "lekhak = 1" (FK `books.author_id`). Lekhak ke register me "meri kitaabein" ki
list bhi hai, par wo bas aaina. Asli sach kitaab ki mohar me.

- Mohar jiske paas = **owning side** = Book (`@ManyToOne @JoinColumn(name = "author_id")`).
- Lekhak ki list = aaina = **inverse side** (`@OneToMany(mappedBy = "author")`: "mohar Book ke `author` field me").

Sirf lekhak ki list me kitaab likhi, mohar nahi lagayi -> DB me `author_id` NULL. Hibernate sirf mohar padhta.

## Tasveer

```
authors            books (mohar = FK)
id  name           id  title   author_id
1   Arpan          1   Java    1
2   Suresh         2   Spring  1
                   3   Python  2

Student <-> Course (many-to-many) = beech ki teesri table: student_course (student_id, course_id)
```

## Dikkat -> kyun

- `mappedBy` kyun: na ho to Hibernate do alag rishte samajhta, faaltu teesri table `author_books` banata.
- Dono side set karo: helper `addBook(b) { books.add(b); b.setAuthor(this); }` (list + mohar saath).

## Trade-off: @ManyToMany ya junction entity

`@ManyToMany` aasaan, par asli kaam me beech ki table pe extra cheez chahiye (enrolledDate, grade) jo wahan nahi aa
sakti. Isliye `Enrollment` entity + do `@ManyToOne`: thoda zyada code, par poora control.

## Prod wala trap (usercrud me tha)

```java
@Entity @Data class Author { @OneToMany(mappedBy = "author") List<Book> books; }
@Entity @Data class Book   { @ManyToOne Author author; }
// log.info(author) -> Author.toString -> books -> Book.toString -> author -> ... StackOverflowError
```

Ek line: entity pe `@Data` nahi (`@Getter @Setter`, `toString` me rishta bahar, equals / hashCode id pe), bahar DTO.
AI bhi entity pe `@Data` lagata hai, PR me ye line pakadna tera kaam.

## Tera project

usercrud: Book owning (`@JoinColumn author_id`), Author inverse (`mappedBy = "author"`).

## Interview line

```
Bidirectional relation me owning side wo hai jiske table me FK hai (@ManyToOne + @JoinColumn),
doosri side mappedBy se sirf aaina. mappedBy bhoolo to extra join table, sirf ek side set karo
to FK NULL, isliye helper method. Many-to-many me main junction entity leta hoon taaki beech
ki table pe extra field aa sakein.
```
