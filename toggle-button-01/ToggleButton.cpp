#include "ToggleButton.h"

ToggleButton::ToggleButton(QWidget* parent): QCheckBox(parent) {
	connect(this, &QCheckBox::stateChanged, this, &ToggleButton::startAnimation);
};

void ToggleButton::resizeEvent(QResizeEvent* event) {
	QCheckBox::resizeEvent(event);
	bodyXPos = 0;
	iconWidth = height() * .3;
}

QVariantAnimation* ToggleButton::getCurrentBodyColorAnimation() {
	QVariantAnimation* animation = new QVariantAnimation(this);
	animation->setEasingCurve(easingCurve);
	animation->setDuration(animationDuration);
	if (isChecked()) {
		animation->setStartValue(currentBodyColor);
		animation->setEndValue(bodyHoverColor);
	}
	else {
		animation->setStartValue(currentBodyColor);
		animation->setEndValue(bodyColor);
	}
	connect(animation, &QVariantAnimation::valueChanged,
		this, [this](const QVariant& variant) { currentBodyColor = variant.value<QColor>(); update(); });
	return animation;
}

QVariantAnimation* ToggleButton::getBodyXPosAnimation() {
	QVariantAnimation* animation = new QVariantAnimation(this);
	animation->setEasingCurve(easingCurve);
	animation->setDuration(animationDuration);
	if (isChecked()) {
		animation->setStartValue(bodyXPos);
		animation->setEndValue(width() - height());
	}
	else {
		animation->setStartValue(bodyXPos);
		animation->setEndValue(0);
	}
	
	connect(animation, &QVariantAnimation::valueChanged,
		this, [this](const QVariant& variant) { bodyXPos = variant.value<int>(); update(); });
	return animation;
}

QVariantAnimation* ToggleButton::getCurrentIconColorAnimation() {
	QVariantAnimation* animation = new QVariantAnimation(this);
	animation->setEasingCurve(easingCurve);
	animation->setDuration(animationDuration);
	if (isChecked()) {
		animation->setStartValue(currentIconColor);
		animation->setEndValue(bodyHoverColor);
	}
	else {
		animation->setStartValue(currentIconColor);
		animation->setEndValue(iconColor);
	}
	connect(animation, &QVariantAnimation::valueChanged,
		this, [this](const QVariant& variant) { currentIconColor = variant.value<QColor>(); update(); });
	return animation;
}

QVariantAnimation* ToggleButton::getIconWidthAnimation() {
	QVariantAnimation* animation = new QVariantAnimation(this);
	animation->setEasingCurve(easingCurve);
	animation->setDuration(animationDuration);
	if (isChecked()) {
		animation->setStartValue(iconWidth);
		animation->setEndValue(2);
	}
	else {
		animation->setStartValue(iconWidth);
		animation->setEndValue(int(height() * 0.3));
	}
	connect(animation, &QVariantAnimation::valueChanged,
		this, [this](const QVariant& variant) { iconWidth = variant.value<int>(); update(); });
	return animation;
}

QRect ToggleButton::getBodyRect() const {
	QRect bodyRect = rect();
	int h = height();
	int offsetY = h * .25;
	int offsetX = height() * .1 + 1;
	bodyRect.adjust(offsetX, offsetY, -offsetX, -offsetY);
	return bodyRect;
}

void ToggleButton::startAnimation() {
	QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
	group->addAnimation(getCurrentBodyColorAnimation());
	group->addAnimation(getBodyXPosAnimation());
	group->addAnimation(getCurrentIconColorAnimation());
	group->addAnimation(getIconWidthAnimation());
	group->start();
	update();
}

void ToggleButton::paintEvent(QPaintEvent *event) {
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing);

	// draw toggle body
	painter.setBrush(currentBodyColor);
	QRect bodyRect = getBodyRect();
	int rayon = bodyRect.height() / 2;
	painter.drawRoundedRect(bodyRect, rayon, rayon);

	// draw shadow
	QRect indicatorRect = QRect(bodyXPos, 0, height(), height());
	int offset = height() * .1;
	indicatorRect.adjust(offset, offset, -offset, -offset);
	QRadialGradient radialGrad(indicatorRect.center(), indicatorRect.height()/1.6);
	radialGrad.setColorAt(.7, Qt::black);
	radialGrad.setColorAt(1, Qt::transparent);
	painter.setOpacity(.05);
	offset = height() * 1.2;
	painter.fillRect(indicatorRect.adjusted(-offset, -offset, offset, offset), radialGrad);
	painter.setOpacity(1);

	// draw toggle indicator
	painter.setBrush(indicatorColor);
	painter.drawEllipse(indicatorRect);

	// draw icon
	offset = height() * .25;
	indicatorRect.adjust(offset, offset, -offset, -offset);
	painter.setBrush(Qt::NoBrush);
	QPen pen = QPen();
	pen.setColor(currentIconColor);
	pen.setWidth(height() * .06);
	painter.setPen(pen);
	offset = (indicatorRect.width() - iconWidth) / 2;
	indicatorRect.adjust(offset, 0, -offset, 0);
	painter.drawRoundedRect(indicatorRect, indicatorRect.width() / 2, indicatorRect.width() / 2);
}