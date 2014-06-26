#include "IPageController.h"
#include "RadioPageController.h"
#include "PlayerPageController.h"
#include "TVPageController.h"

namespace mp {
namespace controller {

IPageControllerPtr CreatePageController(const QString& name)
{
	if(name.compare("tv") == 0)
	{
		return new TVPageController();
	}
	else
	{
		if(name.compare("radio") == 0)
		{
			return new RadioPageController();
		}
		else
		{
			if(name.compare("palyer") == 0)
			{
				return new PlayerPageController();
			}
		}
	}

	return IPageControllerPtr();
}

}
}