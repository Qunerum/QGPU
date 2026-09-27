let docData = null, currentLang = localStorage.getItem('selectedLang') || 'en';
async function loadDocumentation() {
	try {
		const response = await fetch('data.json');
		if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
		docData = await response.json();
		if (!localStorage.getItem('selectedLang')) currentLang = docData.defaultLanguage || 'en';
		renderDocs();
	} catch (error) { console.error("JSON Error:", error); }
}
function renderDocs() {
	if (!docData) return;
	const titleText = getTranslation(docData.title);
	document.title = titleText;
	const titleElement = document.getElementById('doc-title');
	if (titleElement) titleElement.textContent = titleText;
	const categoriesText = document.getElementById('categories'),
		sidebarList = document.getElementById('category-list'),
		mainContainer = document.getElementById('docs-container');
	if (categoriesText) categoriesText.innerHTML = getTranslation(docData.categories);
	if (sidebarList) sidebarList.innerHTML = '';
	if (mainContainer) mainContainer.innerHTML = '';
	if (docData.functions) {
		const categories = Object.keys(docData.functions);
		categories.forEach(categoryName => {
			const funcList = docData.functions[categoryName];
			if (sidebarList) {
				const navItem = document.createElement('a');
				navItem.href = '#';
				navItem.textContent = categoryName;
				navItem.addEventListener('click', (e) => {
					e.preventDefault();
					const targetSection = document.getElementById(`category-${categoryName}`);
					if (targetSection) {
						targetSection.scrollIntoView({ behavior: 'smooth', block: 'start' });
					}
				});
				sidebarList.appendChild(navItem);
			}
			if (mainContainer) {
				const section = document.createElement('section');
				section.className = 'category-section';
				section.id = `category-${categoryName}`;
				const catHeader = document.createElement('h2');
				catHeader.className = 'category-title';
				catHeader.textContent = categoryName;
				section.appendChild(catHeader);
				if (Array.isArray(funcList)) {
					funcList.forEach(fn => {
						let formattedArgs = '';
						if (fn.args) {
							formattedArgs = Object.keys(fn.args).map(argKey => {
								const argDescObj = fn.args[argKey],
									tooltipText = getTranslation(argDescObj),
									parts = argKey.trim().split(/\s+/);
								let argHTML = '';
								if (parts.length > 1) {
									const type = parts.slice(0, -1).join(' ');
									const name = parts[parts.length - 1];
									argHTML = `<span class="syntax-arg-type">${type}</span> <span class="syntax-arg-name">${name}</span>`;
								} else argHTML = `<span class="syntax-arg">${argKey}</span>`;
								return `<span class="arg-wrapper" data-tooltip="${tooltipText}">${argHTML}</span>`;
							}).join('<span class="syntax-punct">, </span>');
						}
						const funcRow = document.createElement('div'),
							description = getTranslation(fn.description);
						funcRow.className = 'func-row';
						funcRow.innerHTML = `
						<code>
						<span class="syntax-return">${fn.returnType}</span>
						<span class="syntax-name">${fn.name}</span><span class="syntax-punct">(</span>${formattedArgs}<span class="syntax-punct">);</span>
						</code>
						<span class="func-separator"></span>
						<span class="func-desc">${description}</span>
						`;
						section.appendChild(funcRow);
					});
				}
				mainContainer.appendChild(section);
			}
		});
	}
}
function getTranslation(translatableObject) {
	if (!translatableObject) return '';
	return translatableObject[currentLang]
	|| translatableObject[docData.defaultLanguage]
	|| Object.values(translatableObject)[0]
	|| 'NULL';
}
function changeLanguage(lang) {
	currentLang = lang;
	localStorage.setItem('selectedLang', lang);
	renderDocs();
}
document.addEventListener('DOMContentLoaded', loadDocumentation);
