#pragma once

#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtCore/QTimer>

using namespace QtDataVisualization;

/**
 * @brief Scatter - klasa której instancja służy do wizualizacji wykresów w przestrzeni 3D
 */

class Scatter : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Scatter - konstruktor klasy scatter
     * @param scatter [Q3DScatter*] - wskaźnik na obiekt typu Q3DScatter, który dostarcza metody pozwalające na wizualizacę danch w przestrzeni.
     */
    explicit Scatter(Q3DScatter *scatter);

    /**
     * @brief destruktor klasy Scatter
    */

    ~Scatter();

    /**
     * @brief generateAndRenderVectors funkcja, która generuje dane na podstawie parametrów wejściowych, potrzebne do przedstawienia ich w przestrzeni.
     */

    void generateAndRenderVectors();

public Q_SLOTS:

    /**
     * @brief setXFirst - ustawia początek osi x
     * @param x - wartość od której ma rozpoczynać się oś x
     */

    void setXFirst(const QString& x);

    /**
     * @brief setXSecond - ustawia koniec osi x
     * @param x - wartość na której ma kończyć się oś x
     */

    void setXSecond(const QString& x);

    /**
     * @brief setYFirst - ustawia początek osi y
     * @param y - wartość od której ma rozpoczynać się oś y
     */

    void setYFirst(const QString& y);

    /**
     * @brief setYSecond - ustawia koniec osi y
     * @param y - wartość na której ma kończyć się oś y
     */

    void setYSecond(const QString& y);

    /**
     * @brief setZFirst - ustawia początek osi z
     * @param z - wartość od której ma rozpoczynać się oś z
     */

    void setZFirst(const QString& z);

    /**
     * @brief setZSecond - ustawia koniec osi z
     * @param z - wartość na której ma kończyć się oś z
     */

    void setZSecond(const QString& z);

    /**
     * @brief setA - ustawia stałą stojącą przy zmiennej x wyjściowego wektora
     * @param a - nowa wartość stałej
     */

    void setA(const QString& a);

    /**
     * @brief setB - ustawia stałą stojącą przy zmiennej y wyjściowego wektora
     * @param b - nowa wartość stałej
     */

    void setB(const QString& b);

    /**
     * @brief setC - ustawia stałą stojącą przy zmiennej c wyjściowego wektora
     * @param c - nowa wartość stałej
     */

    void setC(const QString& c);

    /**
     * @brief setXRange - pozwala zmienić liczbę podprzedziałów na kierunku X
     * @param x - nowa liczba podprzedziałów
     */

    void setXRange(const QString& x);

    /**
     * @brief setYRange - pozwala zmienić liczbę podprzedziałów na kierunku Y
     * @param y - nowa liczba podprzedziałów
     */

    void setYRange(const QString& y);

    /**
     * @brief setZRange - pozwala zmienić liczbę podprzedziałów na kierunku Z
     * @param z - nowa liczba podprzedziałów
     */

    void setZRange(const QString& z);

    /**
     * @brief setArrowsLength - metoda pozwalająca na manipulację długością strzałek
     * @param arrowLength - nowa długość strzałek
     */

    void setArrowsLength(int arrowLength);

    /**
     * @brief functionboxItemChanged - metoda która pozwala zmienić funkcję, za pomocą której wyznaczane są wektory
     * @param index - indeks nowej funkcji
     */

    void functionboxItemChanged(int index);

    /**
     * @brief lengthboxItemChanged - metoda która zmienia tryb wyznaczania długości wektorów.
     * @param index - indeks trybu
     */

    void lengthboxItemChanged(int index);

    /**
     * @brief themeboxItemChanged - metoda która zmienia kolor tła
     * @param index - indeks koloru tła
     */

    void themeboxItemChanged(int index);

    /**
     * @brief setCutByPlain - metoda który pozwala na odcięcie wektorów powyżej zdefiniowanej płaszczyzny
     * @param checked - true - wektory będą odcinane, false - brak odcinania
     */

    void setCutByPlain(bool checked);

    /**
     * @brief setPlainA - ustawia parametr A dla płaszczyny która będzie odcinać wektory
     * @param A - wartość parametru A
     */

    void setPlainA(const QString& A);

    /**
     * @brief setPlainB - ustawia parametr B dla płaszczyny która będzie odcinać wektory
     * @param B - wartość parametru B
     */

    void setPlainB(const QString& B);

    /**
     * @brief setPlainC - ustawia parametr C dla płaszczyny która będzie odcinać wektory
     * @param C - wartość parametru C
     */

    void setPlainC(const QString& C);

    /**
     * @brief setPlainD - ustawia parametr D dla płaszczyny która będzie odcinać wektory
     * @param D - wartość parametru D
     */

    void setPlainD(const QString& D);

    /**
     * @brief handleButton - metoda która obsługuje kliknięcie przycisku zapis do pliku
     */

    void handleButton();
private:
    Q3DScatter *m_graph;

private:

    /**
     * @brief isAbovePlain - metoda która determinuje czy wektor znajduje się nad płaszczyną
     * @param x - parametr x testowanego wektora
     * @param y - parametr y testowanego wektora
     * @param z - parametr z testowanego wektora
     * @return true -> wektor nad płaszczyną, false -> wektor pod płaszczyzną
     */

    bool isAbovePlain(float x, float y, float z);

    /**
     * @brief m_function - zmienna która przechowuje funkcje według której aktualnie wyznaczane są wektory
     */

    std::function<QVector3D(const QVector3D&&, float, float, float)> m_function;

    /**
     * @brief m_xRange - przedział zmienności X
     */

    QPair<float, float> m_xRange;

    /**
     * @brief m_yRange - przedział zmienności Y
     */

    QPair<float, float> m_yRange;

    /**
     * @brief m_zRange - przedział zmienności Z
     */

    QPair<float, float> m_zRange;

    /**
     * @brief m_a - stała przy parametrze x wektora
     */

    float m_a = 1.0f;

    /**
     * @brief m_b - stała przy parametrze y wektora
     */

    float m_b = 1.0f;

    /**
     * @brief m_c - stała przy parametrze z wektora
     */

    float m_c = 1.0f;

    /**
     * @brief m_plainA - parametrz przy zmiennej X płaszczyzny
     */

    float m_plainA = 1.0f;

    /**
     * @brief m_plainB - parametrz przy zmiennej Y płaszczyzny
     */

    float m_plainB = 1.0f;

    /**
     * @brief m_plainC - parametrz przy zmiennej Z płaszczyzny
     */

    float m_plainC = 1.0f;

    /**
     * @brief m_plainA - wyraz wolny płaszczyzny
     */

    float m_plainD = 1.0f;

    /**
     * @brief m_lenghtOption - id domyślnej opcji długości wektrów
     */

    int m_lenghtOption = 0;

    /**
     * @brief m_cutByPlain - parametr decydujący czy płaszczyzna odcina wektory czy nie
     */

    bool m_cutByPlain = false;

    /**
     * @brief m_arrowLength - aktualna długość wektorów
     */

    int m_arrowLength;
};
