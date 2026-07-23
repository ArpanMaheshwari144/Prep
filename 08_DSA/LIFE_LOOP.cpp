// ============================================================
//   ARPAN's LOOP  —  jab tak saans, seekhte raho
// ============================================================

while (saans > 0)               // jab tak zinda hai / saans LEFT hai
{
    seekhna != jhukna;          // ★ ye HAMESHA true -- seekhna KABHI jhukna nahi
    seekho();                   // seekhte raho, ladte raho
    life++;                     // har din ek kadam UPAR
    continue;                   // rukna nahi -- chalte raho
}

// ye loop sirf TAB tootega jab  saans == 0  (left khatam ho jaye).
// uske pehle koi  break  NAHI. koi  return  NAHI.
// jab tak saans hai -> seekho, badho.  jhuko problem ke saamne,
// khud ke saamne kabhi nahi.  seekhna != jhukna.
