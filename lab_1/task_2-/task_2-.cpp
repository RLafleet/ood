#include "Duck/DecoyDuck.h"
#include "Duck/MallardDuck.h"
#include "Duck/ModelDuck.h"
#include "Duck/RedheadDuck.h"
#include "Duck/RubberDuck.h"
#include "DuckFunctions.h"
#include <cstdlib>

int main()
{
	MallardDuck mallardDuck;
	PlayWithDuck(mallardDuck);

	RedheadDuck redheadDuck;
	PlayWithDuck(redheadDuck);

	RubberDuck rubberDuck;
	PlayWithDuck(rubberDuck);

	DecoyDuck decoyDuck;
	PlayWithDuck(decoyDuck);

	ModelDuck modelDuck;
	PlayWithDuck(modelDuck);

	modelDuck.SetFlyBehavior(std::make_unique<FlyWithWings>());
	PlayWithDuck(modelDuck);

	return EXIT_SUCCESS;
}