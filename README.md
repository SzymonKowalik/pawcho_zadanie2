### Zadanie 2
Utworzono łańcuch w usłudze GitHub Actions, który buduje wieloarchitekturowy obraz z wykorzystaniem cache, analizuje go pod kątem podatności na zagrożenia oraz umieszcza na rejestrze ghcr.

Łańcuch, wraz z niezbędnymi komentarzami umieszczony został w pliku `.github/workflows/main.yml`.

#### Konfiguracja środowiska (Zmienne i Sekrety)
- `vars.DH_USERNAME` - nazwa użytkownika DockerHub
- `vars.DH_REPO` - nazwa repozytorium DockerHub
- `secrets.DH_PAT` - Token dostępu do DockerHub
- `secrets.GH_PAT` - Token dostępu do ghcr

#### Analiza podatności CVE
Do wykonania analizy podatności zbudowano lokalny obraz. Następnie przetestowano go. Jeśli sprawdzany obraz zawiera zagrożenia wysokie lub krytyczne potok zostanie przerwany.

#### Tagowanie obrazów
Obraz przesłany do repozytorium ghcr zostanie automatycznie tagowany jako:
- SHA commitu - gwarantuje niezmienność i identyfikowalność kodu, powiązany jest z konkretnym stanem np. `sha-a27ab86`
- Wersja semantyczna - ustawiana automatycznie przy publikacji tagu w Git, co pozwala na powiązanie obrazu z wersją aplikacji np. `v1.0.0`

W żadnym przypadku utworzony obraz nie będzie nazwany jako `latest`. Nazwa ta może być myląca, gdyż wskazuje na ostatnio utworzony obraz, a nie najnowszą wersję obrazu.

#### Zarządzanie cache
Wykorzystywany cache umieszczany jest w repozytorium na platformie DockerHub, co sprawia że utrzymujemy czystość rejestru ghcr.

Cache wykorzystuje stały tag `:buildcache`. Warstwy z historycznych wydań aplikacji stają się osierocone i zostają usuwane poprzez DockerHub, co zapobiega nieskończonemu wzrostowi rozmiarów cache, jednocześnie gwarantując możliwość wykorzystania go przy budowie zaktualizowanych obrazów.

#### Odnośniki do repozytoriów

* [Repozytorium na cache w Docker Hub](https://hub.docker.com/r/szymonk44/zadanie2/tags)
* [Rejestr kontenerów GitHub (GHCR)](https://github.com/users/SzymonKowalik/packages/container/package/pawcho_zadanie2)

