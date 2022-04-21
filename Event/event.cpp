#include "event.h"

int event::getDay(){
    switch (eventType)
    {
    case Formulation:
        return data.form.formulationDay;
    
    case Cancellation:
        return data.cancel.cancellationDay;
    case Promotion:
        return data.promote.promotionDay;
    }
}