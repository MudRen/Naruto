// gender.c

string gender_self(mixed subject)
{
	switch(objectp(subject) ? subject->query("gender") : subject) {
		case "female": return "��";	break;
		default: return "��";
	}
}

string gender_pronoun(string subject)
{
	switch(objectp(subject) ? subject->query("gender") : subject) {
		case "deity":	return "�k";	break;
		case "male":	return "��";	break;
		case "female":	return "��";	break;
		case "male-animal":
		case "female-animal":	return "��";	break;
		default: return "��";
	}
}

