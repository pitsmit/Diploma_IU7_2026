#include "ApiBase.h"

namespace api
{

ApiBase::ApiBase(const std::shared_ptr<Pistache::Rest::Router>& rtr) : router(rtr)
{
}

}