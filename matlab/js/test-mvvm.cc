#include <functional>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>

class Model {
	std::string display_str_;
};

class Message {
public:
	Message(std::string command) : command_(command) {}
	const std::string& command() { return command_; }
	Message() {};

private:
	std::string command_;
	// std::shared_ptr<Model> target_;
};



class ViewModel {
public:

	void OnClick(Message msg) {
		std::cout << msg.command() << std::endl;
	}

	std::shared_ptr<Model> target_;
};


class Window {
public:
	using Callback = std::function<void(Message)>;
	void Register(std::string str, Callback f) {
		callbacks_[str] = f;
	}

	void Update() {
		Message msg;
		for (auto& cb : callbacks_) {
			cb.second(msg);
		}
	}

	void Fire(std::string str, Message msg = Message{ std::string("Hello World") }) {
		auto it = callbacks_.find(str);
		if (it != callbacks_.end()) {
			it->second(msg);
		}
	}

private:
	std::map<std::string, Callback> callbacks_;
};

void TestMVVM()
{
	Window window;
	ViewModel view_model;

	window.Register("Button.Click", [&](Message msg) {
		view_model.OnClick(msg);
	});

	window.Fire("Button.Click");
	getchar();
}