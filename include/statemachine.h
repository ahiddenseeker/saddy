#include "types.h"

int high_score, current_score;
int player_healt_point;
bool paused;


class StateHandler {
public:
	/*! ������� ������������ ������� �� ��������� prev_state � ��������� ������������ �������� �������� ���� �����
	*	\param[in] prev_state ��������� �� �������� ����� ����������
	*	\return ������ ���� ������� �������.
	*/
	virtual bool _operator(int prev_state);
	StateHandler(bool (*function)(int));
	virtual ~StateHandler();
private:
	bool (*m_operator_inner)(int);
};

class StateMachine {
public:
	/*!	������� ������ ���������� ��� ���������� ���������
	*	\param[in] state ��� ����������� ���������, ������ ���� ������ ����
	*	\param[in] handler ��������� �� ������ ���������� ���������
	*/
	void bindState(int state, StateHandler *handler);
	/*!	������� ������������ ������� � ��������� ���������
	*	\param[in] state ��������� � ������� �������
	*	\return ������ ���� ������� �������
	*/
	bool pushState(int state);
	StateMachine();
protected:
	int m_state;//code of state -1 if not state now, else positive number
	hhash<int, StateHandler*> m_state_list;
};