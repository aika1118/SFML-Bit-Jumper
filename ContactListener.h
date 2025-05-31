// File: ContactListner.h
// Purpose: �浹�� ������ ��ü�� ����Ͽ� ���

#pragma once

class b2Fixture;

class ContactListener
{
public:
	/**
	 * �� ���� fixture�� ó������ �������� �� ȣ��Ǵ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� ContactListener Ŭ������ �Ļ� Ŭ�������� �����Ǿ�� �մϴ�.
	 *
	 * @param self  ������ fixture �� �ϳ��Դϴ�.
	 * @param other ������ fixture �� �ٸ� �ϳ��Դϴ�.
	 *
	 * @return ����
	 */
	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) = 0;
	/**
	 * �� ���� fixture�� ������ ������ �и��Ǿ��� �� ȣ��Ǵ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� ContactListener Ŭ������ �Ļ� Ŭ�������� �����Ǿ�� �մϴ�.
	 *
	 * @param self  ������ ���� fixture �� �ϳ��Դϴ�.
	 * @param other ������ ���� fixture �� �ٸ� �ϳ��Դϴ�.
	 *
	 * @return ����
	 */
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) = 0;
	/**
	 * ContactListener Ŭ������ ���� ���� �Ҹ����Դϴ�.
	 *
	 *
	 * @return ����
	 */
	virtual ~ContactListener() = 0; // ���� ���� �Ҹ��ڿ��� ��ü �Ҹ� �� �ݵ�� ȣ��Ǿ���ϱ� ������ ������ ���ǰ� �ʿ���
};