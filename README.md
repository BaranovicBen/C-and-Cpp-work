# Repozitář PA2

Ahoj studente,

toto je tvůj oficiální repozitář k předmětu PA2. Repozitář ti byl vygenerován automaticky, dle doporučené struktury, ale ty si ji můžeš upravit dle svého uvážení. Případně to zkonzultuj se svými cvičícími.

Repozitář již obsahuje následující adresáře:

```
homework/ ...... domácí úlohy
practice/ ...... procvičovací úlohy
```

Využívat jej je nepovinné, ale doporučované.

## Co tu je za soubory?

Všimni si, že soubor `README.md` má v systému Gitlab své speciální místo. Zobrazuje se při procházení adresáři a měl by obsahovat nějaké stručné informace.

Dále věnuj pozornost souborům `.gitkeep`. Nástroj `git` bohužel umí verzovat jen soubory, nikoli adresáře, takže aby bylo možné verzovat prázdný adresář, musíš do něj něco vložit. A běžně se můžeš setkat s podobnými soubory jako `.gitkeep`, které slouží přesně k tomuto účelu. Samozřejmě je můžeš smazat, jen si pak budeš muset složku případně vytvořit ručně.

Můžeš tu najít také soubor `.gitignore`, ten je velice důležitý. Definuje, jaké soubory se nemají verzovat. My jsme ho pro tebe už vyplnili doporučeným nastavením pro jazyk C/C++. Takže například soubory, které končí na `.so` nebo `.o` nebo `.out` se verzovat nebudou. Ale samozřejmě si ho můžeš upravit podle sebe. (Například přidat si podpůrné soubory pro vaše IDE.)

> Při verzování se vždy zamysli, zda soubor verzovat musíš. Binární soubory, které vygeneruješ ze zdrojového kódu kompilací je zbytečné verzovat, vždy totiž můžeš zdrojový kód znovu zkompilovat.

Poslední soubor, který tu najdeš je `.editorconfig`. Ten se netýká Gitu, ale mnoho editorů podporuje tento formát pro nastavení základního coding-style. My už jsme tam hodili nějaká doporučená nastavení, ale klidně si jej uprav podle sebe.

## Jak s tím GITem pracovat?

Je to jednoduché, vždy můžeš s nástrojem `git` pracovat v příkazové řádce, ale většina IDE ti umožní pracovat přímo z editoru. My ti tu ukážeme pár základních příkladů.

- `git clone git@gitlab.fit.cvut.cz:BI-PA2/B202/<username>.git`

	Tento příkaz slouží pro první zkopírování si tvého adresáře ze systému Gitlab pomocí SSH. Místo `<username>` musíš dosadit svoje uživatelské jméno. Ale pro správnou funkčnost potřebuješ mít nastavený svůj veřejný klíč v systému Gitlab. Alternativně můžeš použít HTTPS připojení, odkaz najdeš ve svém repozitáři pod tlačítkem _Clone_.

- `git add main.cpp`

	Tímto příkazem řekneš nástroji `git`, aby tvůj soubor `main.cpp` verzoval. Tedy hlídal změny a případně propagoval tyto změny do tvého repozitáře.

- `git commit -m "Fixed memory-leaks in 2nd homework"`

  Tímto příkazem takzvaně _commitneš_ změny (vytvoříš novou verzi). Nástroj `git` si uloží aktuální stav tvých souborů do své interní databáze. V tuto chvíli, ale pracuješ jen se svým lokálním repozitářem. Tento příkaz se hodí volat prakticky po každém smysluplném bloku (a samozřejmě musíš napsat nějakou smysluplnou zprávu).

- `git push`

  Dosud jsme pracovali jen se svým lokálním repozitářem, ale když skončíš svoji práci, je dobré ji odeslat na server. Voláním tohoto příkazu pošleš své změny na server Gitlab a budeš je mít tak zálohované. Doporučujeme ti volat tento příkaz vždy po konci své práce.

- `git pull`

  Pokud s GITem začínáš, tak tento příkaz asi nebudeš potřebovat. Hodí se ti tehdy, když pracuješ na více strojích. Před začátkem práce si vždy stáhneš ze systému Gitlab tímto příkazem svoji práci. A po skončení ji příkazem `pull` zase nahraješ zpět. V případě, že by nebylo provedeno na začátku volání `pull`, mohlo by se stát, že budeš muset řešit tzv. _konflikty_ a provést _merge_ (sloučení změn). Ale to se ti při práci v PA2 pravděpodobně nestane.

> Tohle je opravdu jednoduchý přehled, podrobné informace o tom jak naplno a správně používat nástroj GIT můžeš získat na předmětu [BI-GIT](https://courses.fit.cvut.cz/BI_GIT/), který ti vřele doporučujeme. Pokud by ses ovšem na něčem zasekl, zeptej se svých cvičících.
