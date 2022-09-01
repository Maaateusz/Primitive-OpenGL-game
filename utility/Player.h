#ifndef __CPLAYER
#define __CPLAYER

// ----------------------------------------------------------------
// Klasa do reprezentacji obiektu, ktory porusza sie po scenie
class CPlayer
{

public:

	// skladowe
    glm::vec3 Position = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 Direction;
    float Angle;

	// macierz modelu
	glm::mat4 matModel;


    CPlayer() { }

    // Inicjalizacja obiektu
    void Init()
    {
    	// TO_DO: Uzupelnij wedle uznania
    }

	// Obliczenie wysokosci nad ziemia
	void UpdateLatitute()
	{
		// TO_DO: Wykorzystaj klase CGround i zaktualizuj
		// wspolrzedna Y obiektu zgodnie z podlozem
	}


	// aktualizacja macierzy modelu
	void UpdateMatModel()
	{
		matModel = glm::translate(glm::mat4(1.0), Position);
		matModel = glm::rotate(matModel, Angle, glm::vec3(0.0, 1.0, 0.0));
	}

    // ustawienie pozycji na scenie
    void SetPosition(glm::vec3 pos)
    {
        Position = pos;
        UpdateMatModel();
    }

    // zmiana pozycji na scenie
    void Move(float val)
    {
        Position += Direction * val;

        // aktualizacja danych i macierzy
        UpdateLatitute();
        UpdateMatModel();
    }

    void Rotate(float angle)
    {
    	Angle += angle;
		Direction.x = cos(Angle);
		Direction.z = -sin(Angle);

		// aktualizacja
		UpdateMatModel();
    }

};


#endif
