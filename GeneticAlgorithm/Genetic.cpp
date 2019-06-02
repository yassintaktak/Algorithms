#include <iostream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

/*
	MAIN CONFIGURATION
*/
string target = "To be or not to be";
int max_population = 200;
float mutationRate = 2;
int generationCount = 0;
/*
	END MAIN CONFIGURATION
*/




int randomInteger(int min, int max) {
	return min + rand() % ((max + 1) - min);
}

float mapValue(float value, float mn1, float mx1, float mn2, float mx2) {
	return mn2 + (((value - mn1)*(mx2 - mn2)) / (mx1 - mn1));
}

class DNA {
public:
	DNA(int length, string target) {
		m_length = length;
		m_target = target;
		m_genes = new char[m_length];
		for (int i = 0; i < m_length; i++) {
			m_genes[i] = generateRandomChar();
		}
		calcFitness();
	}

	DNA(int length, string target, string genes) {
		m_length = length;
		m_target = target;
		m_genes = new char[m_length];
		for (int i = 0; i < m_length; i++) {
			m_genes[i] = genes[i];
		}
		calcFitness();
	}

	string getPhrase() {
		string tempPhrase;
		for (int i = 0; i < m_length; i++) {
			tempPhrase += m_genes[i];
		}
		return tempPhrase;
	}

	char generateRandomChar() {
		int randAsciiNumber = randomInteger(63, 122);
		char randChar = randAsciiNumber;
		if (randChar == 63)
			randChar = 32;
		if (randChar == 64)
			randChar = 46;

		return randChar;
	}

	void calcFitness() {
		int score = 0;

		for (int i = 0; i < m_length; i++) {
			if (target[i] == m_genes[i]) {
				score += 1;
			}
		}
		m_fitness = (float) score / (float)m_length;
	}

	float getFitness() {
		return m_fitness;
	}

	string getGenesToPoint(int from, int to) {
		string copyGenes = "";
		for (int i = from; i < to; i++) {
			copyGenes += m_genes[i];
		}
		return copyGenes;
	}
private:
	char* m_genes;
	int m_length;
	float m_fitness;
	string m_target;
};


/*
	START VARIABLES
*/
vector<DNA> population;
vector<DNA> breeding_pool;
/*
	END VARIABLES
*/

void populate() {
	population.clear();
	for (int i = 0; i < max_population; i++) {
		population.push_back(DNA(target.size(), target));
	}
}

void crossOver() {
	for (int i = 0; i < max_population; i++) {
		DNA parentDnaOne = breeding_pool[randomInteger(0, breeding_pool.size() - 1)];
		DNA parentDnaTwo = breeding_pool[randomInteger(0, breeding_pool.size() - 1)];
		int randomPointCrossOne = randomInteger(0, target.size());
		string gene_one = parentDnaOne.getGenesToPoint(0, randomPointCrossOne) + parentDnaTwo.getGenesToPoint(randomPointCrossOne, target.size());
		for (int i = 0; i < gene_one.size(); i++) {
			if (randomInteger(0, 100) < mutationRate) {
				gene_one[i] = parentDnaOne.generateRandomChar();
			}
		}

		DNA childDnaOne = DNA(target.size(), target, gene_one);
		population.push_back(childDnaOne);
	}
}

void breed() {
	generationCount += 1;
	breeding_pool.clear();
	for (int i = 0; i < max_population; i++) {
		DNA tmpDna = population[i];
		int svalue = floor(mapValue(tmpDna.getFitness(), 0, 1, 0, 100));
		//cout << population[i].getPhrase() << " : " << population[i].getFitness() << " : " << svalue << endl;
		for (int j = 0; j < svalue; j++) {
			breeding_pool.push_back(tmpDna);
		}
	}
	
	population.clear();

	crossOver();
}

bool checkSolution() {
	float maxFitness = 0;
	string maxPhrase = "";
	for (int i = 0; i < population.size(); i++) {
		if (population[i].getPhrase() == target) {
			cout << "Phrase found ! " << population[i].getPhrase() << " Generation : " << generationCount << endl;
			return true;
		}
		if (population[i].getFitness() > maxFitness) {
			maxFitness = population[i].getFitness();
			maxPhrase = population[i].getPhrase();
		}
	}
	cout << "Current best phrase : " << maxPhrase << " : " << maxFitness << endl;
	return false;
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	populate();
	while (true && generationCount < 2000) {
		cout << "Generation " << generationCount << endl;
		breed();
		if (checkSolution()) {
			break;
		}
	}
	return 0;
}
