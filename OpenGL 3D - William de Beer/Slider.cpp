#include "Slider.h"
#include "Utilities.h"

Slider::Slider(glm::vec2 _pos, glm::vec2 _size, int _minValue, int _maxValue, int _startValue, std::string _label)
{
	m_CentrePoint = _pos;
	m_iCurrentValue = _startValue;
	m_Backing = new CButton();
	m_Backing->Initialise(_pos + glm::vec2(0.17f, 0.0f), glm::vec2(0.17f, _size.y), 1);
	m_Knob = new CButton();

	m_MinValue = _minValue;
	m_MaxValue = _maxValue;

	float screenWidth = CUtilities::GetInstance().GetResolution().x;
	float screenHeight = CUtilities::GetInstance().GetResolution().y;

	float percentage = (float)(m_iCurrentValue) / (float)(m_MaxValue - m_MinValue);
	float minPos = m_CentrePoint.x;
	float maxPos = m_CentrePoint.x + (2.0f * 200.0f / screenWidth);

	float newX = (percentage * (maxPos - minPos)) + minPos;


	//float minPos = m_CentrePoint.x;
	//float maxPos = m_CentrePoint.x + (2.0f * 200.0f / screenWidth);
	//float newX = glm::clamp(percentage * (maxPos - minPos), minPos, maxPos);

	//m_Knob->SetPosition(glm::vec3(newX, m_Knob->GetPosition().y, 0.0f));
	m_Knob->Initialise(glm::vec2(newX, m_CentrePoint.y), _size, 1);
	m_Knob->SetTexture("Cube.png");

	std::string font = "Resources/Fonts/VERDANA.TTF";
	std::string Text = std::to_string(m_iCurrentValue);

	glm::vec2 textPosition = glm::vec2(0.0f, 0.0f);
	textPosition.x = ((maxPos + 1.0f) / 2.0f) * screenWidth + 12.0f;
	textPosition.y = ((_pos.y + 1.0f) / 2.0f) * screenHeight - 4.0f;
	m_Text = new CText(Text, font, glm::vec2(12.0f, 12.0f), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	
	glm::vec2 labelPosition = glm::vec2(0.0f, 0.0f);
	labelPosition.x = ((_pos.x + 1.0f) / 2.0f) * screenWidth + 6.0f;
	labelPosition.y = ((_pos.y + 1.0f) / 2.0f) * screenHeight + 14.0f;
	m_Label = new CText(_label, font, glm::vec2(12.0f, 12.0f), labelPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	
}

Slider::~Slider()
{
	if (m_Knob != nullptr)
	{
		delete m_Knob;
		m_Knob = 0;
	}
	if (m_Backing != nullptr)
	{
		delete m_Backing;
		m_Backing = 0;
	}
	if (m_Label != nullptr)
	{
		delete m_Label;
		m_Label = 0;
	}
	if (m_Text != nullptr)
	{
		delete m_Text;
		m_Text = 0;
	}
}

int Slider::Update(float _dT)
{
	float screenWidth = CUtilities::GetInstance().GetResolution().x;
	float screenHeight = CUtilities::GetInstance().GetResolution().y;

	float MouseX = (2.0f * (float)CInputHandle::GetInstance().GetMouseX()) / screenWidth - 1.0f;
	float MouseY = 1.0f - (2.0f * (float)CInputHandle::GetInstance().GetMouseY()) / screenHeight;

	float minPos = m_CentrePoint.x;
	float maxPos = m_CentrePoint.x + (2.0f * 200.0f / screenWidth);
	float newX = glm::clamp(MouseX, minPos, maxPos);

	minPos = (minPos + 1.0f);
	maxPos = (maxPos + 1.0f);
	newX = (newX + 1.0f);

	if (m_Backing->ActivateButton())
	{
		std::cout << MouseX << ", " << minPos << ", " << maxPos << std::endl;

		m_Knob->SetPosition(glm::vec3((newX - minPos) + m_CentrePoint.x, m_Knob->GetPosition().y, 0.0f));
		float percentage = ((newX - minPos) / (maxPos - minPos));
		m_iCurrentValue = (percentage * (m_MaxValue - m_MinValue)) + m_MinValue;
		//CUtilities::GetInstance().SetClothSize(glm::vec2(m_iCurrentValue, m_iCurrentValue));
	}


	std::string Text = std::to_string(m_iCurrentValue);
	m_Text->SetText(Text);
	m_Backing->Process();
	m_Knob->Process();

	return m_iCurrentValue;
}

void Slider::Render(GLuint _program, CCamera* _cam)
{
	m_Backing->Draw(_program, _cam);
	m_Knob->Draw(_program, _cam);
	m_Text->Render();
	m_Label->Render();
}
