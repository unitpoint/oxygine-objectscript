#include "oxygine-framework.h"
#include "res.h"
#include "Game.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameMenu.h"

using namespace oxygine;

#include <objectscript.h>
#include <os-binder.h>
#include <ext-datetime/os-datetime.h>

class OxygineOS: public ObjectScript::OS
{
public:

	OxygineOS()
	{
	}

	virtual bool isFileExist(const OS_CHAR * filename)
	{
		return file::exists(filename);
	}

	/* virtual int getFileSize(const OS_CHAR * filename)
	{
	}

	virtual int getFileSize(FileHandle * f)
	{
	} */
	
	virtual FileHandle * openFile(const OS_CHAR * filename, const OS_CHAR * mode)
	{
		return (FileHandle*)file::open(filename, mode);
	}

	virtual int readFile(void * buf, int size, FileHandle * f)
	{
		if(f){
			return file::read((file::handle)f, buf, (unsigned)size);
		}
		return 0;
	}

	virtual int writeFile(const void * buf, int size, FileHandle * f)
	{
		if(f){
			file::write((file::handle)f, buf, (unsigned)size);
		}
		return size;
	}

	virtual int seekFile(FileHandle * f, int offset, int whence)
	{
		if(f){
			return file::seek((file::handle)f, offset, whence);
		}
		return 0;
	}
	
	virtual void closeFile(FileHandle * f)
	{
		if(f){
			file::close((file::handle)f);
		}
	}

	virtual void echo(const void * buf, int size)
	{
		const char * output_filename = "out.txt";
		FileHandle * f = openFile(output_filename, "a");
		OS_ASSERT(f);
		writeFile((const char*)buf, size, f);
		closeFile(f);
	}
};

namespace ObjectScript {

template <class T> struct CtypeOXClass{};
template <class T> struct CtypeOXClass<T*>
{
	typedef typename RemoveConst<T>::type ttype;
	typedef typename RemoveConst<T>::type * type;

	static bool isValid(const type p){ return p != NULL; }
	static type def(ObjectScript::OS*){ return type(); }
	static type getArg(ObjectScript::OS * os, int offs)
	{
		const OS_ClassInfo& classinfo = ttype::getClassInfoStatic();
		return (type)os->toUserdata(classinfo.instance_id, offs, classinfo.class_id);
	}
	static void push(ObjectScript::OS * os, const type& val)
	{
		if(!val){
			os->pushNull();
			return;
		}
		val->addRef();
		const OS_ClassInfo& classinfo = val->getClassInfo();
		os->pushUserPointer(classinfo.instance_id, val, UserDataDestructor<ttype>::dtor);
		os->pushStackValue();
		os->getGlobal(classinfo.classname);
		if(!os->isUserdata(classinfo.class_id, -1)){
			OX_ASSERT(false);
			os->pop(2);
		}else{
			os->setPrototype(classinfo.instance_id);
		}
	}
};

template <class T> struct CtypeOXSmartClass
{
	typedef typename T::element_type ttype;
	typedef typename T::element_type * type;

	static bool isValid(const type p){ return p != NULL; }
	static type def(ObjectScript::OS*){ return type(); }
	static type getArg(ObjectScript::OS * os, int offs)
	{
		const OS_ClassInfo& classinfo = ttype::getClassInfoStatic();
		return (type)os->toUserdata(classinfo.instance_id, offs, classinfo.class_id);
	}
	static void push(ObjectScript::OS * os, const T& val)
	{
		if(!val){
			os->pushNull();
			return;
		}
		val->addRef();
		const OS_ClassInfo& classinfo = val->getClassInfo();
		os->pushUserPointer(classinfo.instance_id, val.get(), UserDataDestructor<ttype>::dtor);
		os->pushStackValue();
		os->getGlobal(classinfo.classname);
		if(!os->isUserdata(classinfo.class_id, -1)){
			OX_ASSERT(false);
			os->pop(2);
		}else{
			os->setPrototype(classinfo.instance_id);
		}
	}
};

#define OS_DECL_OX_CLASS(type) OS_DECL_OX_CLASS_NAME(type, #type)
#define OS_DECL_OX_CLASS_NAME(type, name) \
	OS_DECL_CTYPE_NAME(type, name); \
	template <> struct CtypeValue<type*>: public CtypeOXClass<type*>{}; \
	template <> struct CtypeValue< intrusive_ptr<type> >: public CtypeOXSmartClass< intrusive_ptr<type> >{}; \
	template <> struct UserObjectDestructor<type>{ static void dtor(type * p){ p->releaseRef(); } };

OS_DECL_OX_CLASS_NAME(Object, "OxygineObject");
OS_DECL_OX_CLASS(Event);
OS_DECL_OX_CLASS(TouchEvent);
OS_DECL_OX_CLASS(EventDispatcher);
OS_DECL_OX_CLASS(Actor);
OS_DECL_OX_CLASS(Sprite);
OS_DECL_OX_CLASS(RootActor);

// =====================================================================

OS_DECL_CTYPE_NAME(Vector2, "vec2");

template <>
struct CtypeValue<Vector2>
{
	typedef Vector2 type;

	static bool isValid(const Vector2&){ return true; }

	// static type to(const b2Vec2& val){ return (type)val; }
	static Vector2 def(ObjectScript::OS * os){ return Vector2(0, 0); }
	static Vector2 getArg(ObjectScript::OS * os, int offs)
	{
		if(os->isObject(offs)){
			float x = (os->getProperty(offs, "x"), os->popFloat());
			float y = (os->getProperty(offs, "y"), os->popFloat());
			return Vector2(x, y);
		}
#if 0
		if(os->isArray(offs)){
			os->pushStackValue(offs);
			os->pushNumber(0);
			os->getProperty();
			float x = os->popFloat();
		
			os->pushStackValue(offs);
			os->pushNumber(1);
			os->getProperty();
			float y = os->popFloat();

			return Vector2(x, y);
		}
#endif
		os->setException("vec2 required");
		return Vector2(0, 0);
	}

	static void push(ObjectScript::OS * os, const Vector2& p)
	{
#if 1
		os->getGlobal("vec2");
		os->pushGlobals();
		os->pushNumber(p.x);
		os->pushNumber(p.y);
		os->call(2, 1);
#else
		os->newObject();
	
		os->pushStackValue();
		os->pushNumber(p.x);
		os->setProperty("x", false);
				
		os->pushStackValue();
		os->pushNumber(p.y);
		os->setProperty("y", false);
#endif
	}
};

// typedef std::map<std::string, OS_ClassInfo> OS_ClassInfos;
// OS_ClassInfos classinfos;

template <class T>
void registerOXClass(ObjectScript::OS * os, const ObjectScript::OS::FuncDef * list, const ObjectScript::OS::NumberDef * numbers = NULL, bool instantiable = true)
{
	const OS_ClassInfo& classinfo = T::getClassInfoStatic();
	/* OS_ClassInfos::iterator it = classinfos.find(classinfo.classname);
	if(it != classinfos.end()){
		OX_ASSERT(false);
	}
	classinfos.insert(OS_ClassInfos:: classinfo.classname, classinfo);
	*/

	os->pushGlobals();
	os->pushString(classinfo.classname);
	os->pushUserdata(classinfo.class_id, 0, NULL, NULL);
	os->setFuncs(list);
	os->setNumbers(numbers);
	os->pushBool(instantiable);
	os->setProperty(-2, OS_TEXT("__instantiable"), false);
	os->setProperty();
}

template <class T, class Prototype>
void registerOXClass(ObjectScript::OS * os, const ObjectScript::OS::FuncDef * list, const ObjectScript::OS::NumberDef * numbers = NULL, bool instantiable = true)
{
	const OS_ClassInfo& classinfo = T::getClassInfoStatic();
	/* OS_ClassInfos::iterator it = classinfos.find(classinfo.classname);
	if(it != classinfos.end()){
		OX_ASSERT(false);
	}
	classinfos.insert(classinfo.classname, classinfo);
	*/

	os->pushGlobals();
	os->pushString(classinfo.classname);
	os->pushUserdata(classinfo.class_id, 0, NULL, NULL);
	os->setFuncs(list);
	os->setNumbers(numbers);
	os->pushBool(instantiable);
	os->setProperty(-2, OS_TEXT("__instantiable"), false);

	const OS_ClassInfo& classinfo_proto = Prototype::getClassInfoStatic();
	/* it = classinfos.find(classinfo_proto.classname);
	if(it == classinfos.end()){
		OX_ASSERT(false);
	} */

	os->pushStackValue();
	os->getGlobal(classinfo_proto.classname);
	os->setPrototype(classinfo.class_id);
	os->setProperty();
}

struct Oxygine
{
	static void registerObject(OS * os)
	{
		struct Lib {
			static int get(OS * os, int params, int, int, void*)
			{
				os->setException(OS::String::format(os, "property \"%s\" not found in \"%s\"", 
					os->toString(-params+0).toChar(),
					os->getValueNameOrClassname(-params-1).toChar()));
				return 0;
			}
		};

		OS::FuncDef funcs[] = {
			{"__get", &Lib::get},
			{}
		};
		registerOXClass<Object>(os, funcs);
	}

	static void registerEvent(OS * os)
	{
		struct Lib {
		};

		OS::FuncDef funcs[] = {
			def("stopPropagation", &Event::stopPropagation),
			def("stopImmediatePropagation", &Event::stopImmediatePropagation),
			{}
		};
		registerOXClass<Event, Object>(os, funcs);
	}

	static void registerTouchEvent(OS * os)
	{
		struct Lib {
			static int get(OS * os, int params, int, int, void*)
			{
				return 0;
			}
		};

		OS::FuncDef funcs[] = {
			{}
		};
		OS::NumberDef nums[] = {
			{"CLICK", TouchEvent::CLICK},
			{"OVER", TouchEvent::OVER},
			{"OUT", TouchEvent::OUT},
			{"MOVE", TouchEvent::MOVE},
			{"TOUCH_DOWN", TouchEvent::TOUCH_DOWN},
			{"TOUCH_UP", TouchEvent::TOUCH_UP},
			{"WHEEL_UP", TouchEvent::WHEEL_UP},
			{"WHEEL_DOWN", TouchEvent::WHEEL_DOWN},
			{}
		};
		registerOXClass<TouchEvent, Event>(os, funcs, nums);
	}

	static void registerEventDispatcher(OS * os)
	{
		struct Lib {
			static int addEventListener(OS * os, int params, int, int, void*)
			{
				OS_GET_SELF(EventDispatcher*);
				if(params < 2){
					os->setException("two arguments required");
					return 0;
				}
				if(!os->isFunction(-params+1)){
					os->setException("2nd argument must be function");
					return 0;
				}
				int funcId = os->getValueId(-params+1);
				eventType ev;
				switch(os->getType(-params+0)){
				case OS_VALUE_TYPE_NUMBER:
					ev = (eventType)os->toInt(-params+0);
					break;

				case OS_VALUE_TYPE_STRING:
					ev = (eventType)os->toString(-params+0).string->hash; // TODO: change to crc32
					break;

				default:
					os->setException("the first argument should be string or number");
					return 0;
				}
				os->pushNumber(self->addEventListener(ev, EventCallback(os, funcId)));
				return 1;
			}

			static int removeEventListener(OS * os, int params, int, int, void*)
			{
				OS_GET_SELF(EventDispatcher*);
				if(params == 1){
					switch(os->getType(-params+0)){
					case OS_VALUE_TYPE_NUMBER:
						self->removeEventListener(os->toInt(-params+0));
						return 0;

					default:
						os->setException("argument should be number here");
						return 0;
					}
				}
				eventType ev;
				switch(os->getType(-params+0)){
				case OS_VALUE_TYPE_NUMBER:
					ev = (eventType)os->toInt(-params+0);
					break;

				case OS_VALUE_TYPE_STRING:
					ev = (eventType)os->toString(-params+0).string->hash; // TODO: change to crc32
					break;

				default:
					os->setException("the first argument should be string or number here");
					return 0;
				}
				if(!os->isFunction(-params+1)){
					os->setException("2nd argument must be function");
					return 0;
				}
				int funcId = os->getValueId(-params+1);
				self->removeEventListener(ev, EventCallback(os, funcId));
				return 0;
			}
		};

		OS::FuncDef funcs[] = {
			def("__get@listenersCount", &EventDispatcher::getListenersCount),
			{"addEventListener", &Lib::addEventListener},
			{"removeEventListener", &Lib::removeEventListener},
			def("removeAllEventListeners", &EventDispatcher::removeAllEventListeners),
			{}
		};
		registerOXClass<EventDispatcher, Object>(os, funcs);
	}

	static void registerActor(OS * os)
	{
		struct Lib // Actor: public oxygine::Actor
		{
			static Actor * __newinstance()
			{
				return new Actor();
			}
		};

		OS::FuncDef funcs[] = {
			def("__newinstance", &Lib::__newinstance),
			def("addChild", (void(Actor::*)(Actor*))&Actor::addChild),

			def("__get@x", &Actor::getX),
			def("__set@x", &Actor::setX),
			
			def("__get@y", &Actor::getY),
			def("__set@y", &Actor::setY),
			
			def("__get@width", &Actor::getWidth),
			def("__set@width", &Actor::setWidth),
			
			def("__get@height", &Actor::getHeight),
			def("__set@height", &Actor::setHeight),
			
			def("__get@priority", &Actor::getPriority),
			def("__set@priority", &Actor::setPriority),
			
			def("__get@anchor", &Actor::getAnchor),
			def("__set@anchor", (void(Actor::*)(const Vector2 &))&Actor::setAnchor),
			{}
		};
		registerOXClass<Actor, EventDispatcher>(os, funcs);
	}

	static void registerSprite(OS * os)
	{
		struct Lib // Actor: public oxygine::Actor
		{
			static Sprite * __newinstance()
			{
				return new Sprite();
			}

			static int setResAnimTest(OS * os, int params, int, int, void*)
			{
				OS_GET_SELF(Sprite*);
				OS::String name = os->toString(-params+0);
				self->setResAnim(res::ui.getResAnim(name.toChar()));
				return 0;
			}
		};

		OS::FuncDef funcs[] = {
			def("__newinstance", &Lib::__newinstance),
			{"setResAnimTest", &Lib::setResAnimTest},
			{}
		};
		registerOXClass<Sprite, Actor>(os, funcs);
	}

	static void registerRootActor(OS * os)
	{
		OS::FuncDef funcs[] = {
			{}
		};
		registerOXClass<RootActor, Actor>(os, funcs);
	}

	static OxygineOS * os;

	/*
	static int stackOrder;
	static void checkStackOrder(int prev = 0)
	{
		if(prev == 0){
			checkStackOrder((intptr_t)&prev);
		}else{
			int cur = 0;
			stackOrder = (intptr_t)&cur < prev;
		}
	}
	*/

	static void init()
	{
		// checkStackOrder();

		os = OS::create(new OxygineOS());

		initDateTimeExtension(os);

		registerObject(os);
		registerEvent(os);
		registerTouchEvent(os);
		registerEventDispatcher(os);
		registerActor(os);
		registerSprite(os);
		registerRootActor(os);
	}

	static void shutdown()
	{
		os->release();
	}

	static void run()
	{
		// pushCtypeValue(os, getRoot().get());
		pushCtypeValue(os, getRoot());
		os->setGlobal("root");

		os->require("main.os");
	}

}; // struct Oxygine

OxygineOS * Oxygine::os;
// int Oxygine::stackOrder;

} // namespace ObjectScript


void callObjectScriptFunction(ObjectScript::OS * os, int func_id, Event * ev)
{
	int is_stack_event = !ev->_ref_counter; // ((intptr_t)ev < (intptr_t)&ev) ^ ObjectScript::Oxygine::stackOrder;
	if(is_stack_event){
		// ev = ev->clone();
	}
	os->pushValueById(func_id);
	OX_ASSERT(os->isFunction());
	os->pushGlobals(); // this
	pushCtypeValue(os, ev);
	os->call(1);
}

void example_preinit()
{
	ObjectScript::Oxygine::init();
}

void example_init()
{
	//load resources
	res::load();

	//create all scenes
	GameMenu::instance = new GameMenu;
	GameScene::instance = new GameScene;
	MainMenuScene::instance = new MainMenuScene;

	//show main menu
	MainMenuScene::instance->show();

	ObjectScript::Oxygine::run();
}

void example_update()
{

}

void example_destroy()
{
	MainMenuScene::instance = 0;
	GameScene::instance = 0;
	GameMenu::instance = 0;

	ObjectScript::Oxygine::shutdown();

	res::free();
}