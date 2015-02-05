#include "scenenodesschemasetter.h"

#include "../tovalue.h"

#include "../../history/customobject/customobjectchangeschema.h"

#include <renderer.h>

#include <resource/tree.h>
#include <resource/resource.h>

scripting::scenenodes::SchemaSetter::SchemaSetter(QScriptEngine* e)
    : scripting::scenenodes::AbstractSetter<sad::String>(e)
{
	addMatched("schema");
}

scripting::scenenodes::SchemaSetter::~SchemaSetter()
{

}

sad::Maybe<QString> scripting::scenenodes::SchemaSetter::canBeCalled(QScriptContext* ctx)
{
   sad::Maybe<QString> result = this->scripting::scenenodes::AbstractSetter<sad::String>::canBeCalled(ctx);
   if (result.exists() == false)
   {
        sad::Maybe<sad::String> r = scripting::ToValue<sad::String>::perform(ctx->argument(2));
        sad::String resourcename = r.value();
        sad::resource::Resource* resource = sad::Renderer::ref()->tree("")->root()->resource(resourcename);
        bool valid = false;
        if (resource) {
            if (resource->metaData()->canBeCastedTo("sad::db::custom::Schema"))
            {
                valid = true;
            }
        }
        if (!valid)
        {
            result.setValue(QString(resourcename.data()) + " is not a schema resource");
        }
   }
    if (result.exists() == false)
    {
	     sad::Maybe<sad::SceneNode*> r = scripting::ToValue<sad::SceneNode*>::perform(ctx->argument(0));
		 sad::SceneNode* node = r.value();
		 if (node->metaData()->canBeCastedTo("sad::db::custom::Object") == false)
		 {
			 result.setValue("first argument is not a custom object");
		 }
    }
   return result;
}

history::Command* scripting::scenenodes::SchemaSetter::command(
	sad::SceneNode* obj, 
	const sad::String&, 
	sad::String oldvalue,  
	sad::String newvalue
)
{
	return new history::customobject::ChangeSchema(
		static_cast<sad::db::custom::Object*>(obj),
		oldvalue,
		newvalue
	);
}